import os
import re
import math
from collections import defaultdict, Counter
import nltk
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
import string
from utils.document_processor import TextExtractionEngine
from utils.logger import get_logger
import jellyfish
import time
import numpy as np
from sklearn.cluster import KMeans
from sklearn.feature_extraction.text import TfidfVectorizer
import numpy as np

logger=get_logger(__name__)
nltk.download("stopwords")

class VSM:
    """
    Implementation of vector space model for documents, on a directory basis
    """
    def __init__(self, corpus_dir: str, n_clusters=5, ngram_range=(1,2))->None:
        """
        Initialises VSM class

        Args:
            corpus_dir: Directory containing files to be processed
            n_clusters: Number of clusters for K means
            ngram_range: Range for extraction (min, max)
        
        Returns:
            None
        """
        self.corpus_dir = corpus_dir
        self.n_clusters=n_clusters
        self.ngram_range=ngram_range

        self.doc_index={} # document id --> file name mapping
        self.dictionary={} # term --> (document frequency, postings list(document id, term frequency))
        self.doc_lengths={} # document id --> vector length
        self.soundex_dict=defaultdict(list)
        self.N=0 # number of documents

        self.doc_clusters={}
        self.cluster_centers={}
        self.doc_texts={}
        self.phrase_dict=[]

        #preprocessing tools
        self.stop_words=set(stopwords.words("english"))
        self.stemmer=PorterStemmer()
        logger.info(f"Initialized VSM for corpus directory: {corpus_dir}")
    
    def is_meaningful_phrase(self, phrase_tokens: list) -> bool:
            """
            removes meaningless phrases
            """
            if len(phrase_tokens) < 2:
                return False
                
            common_words = {'the', 'and', 'or', 'but', 'in', 'on', 'at', 'to', 'for', 'of', 'with', 'by'}
            meaningful_count = sum(1 for token in phrase_tokens if token not in common_words)
            
            return meaningful_count >= 1

    def extract_phrases(self, tokens: list)->list:
        """
        Extract n-gram tokens aka phrases from tokens

        Args:
            tokens: list of processed tokens
        
        Returns:
            list: meaningful phrases
        """
        phrases=[]
        min_n, max_n, = self.ngram_range

        for n in range(min_n, max_n+1):
            if n==1:
                continue
                
            for i in range(len(tokens)-n+1):
                phrase="_".join(tokens[i:i+n])

                if self.is_meaningful_phrase(tokens[i:i+n]):
                    phrases.append(phrase)
        return phrases

    def preprocess(self, text: str)->list:
        """
        Preprocesses the text by removing stop words, stemming using Porter's algorithms, and returning tokens with n-gram support
        
        Args:
            text: Text to be preprocessed
        
        Returns:
            list: List of tokens and phrases after preprocessing
        """
        text=text.lower()
        text=text.replace("-", " ").replace("_", " ")
        text=text.translate(str.maketrans("", "", string.punctuation))
        tokens=text.split()

        tokens=[
            self.stemmer.stem(t) for t in tokens if t not in self.stop_words and not t.isdigit()
        ]

        phrases=self.extract_phrases(tokens)

        all_terms=tokens + phrases

        logger.info(f"Preprocessed text into {len(all_terms)} tokens and phrases")
        return tokens

    def compute_doc_length(self):
        """
        Computes the vector length for each document in the corpus
        """
        self.doc_lengths=defaultdict(float)
        
        # Include regular terms and phrases
        all_terms=list(self.dictionary.keys()) + list(self.phrase_dict.keys()) if self.phrase_dict else list(self.dictionary.keys())
        
        for term in self.dictionary:
            df, posting_list=self.dictionary[term]
            for docID, tf in posting_list:
                w= 1 + math.log10(tf)
                self.doc_lengths[docID] += w * w #utilising provided scheme
        
        for phrase in self.phrase_dict:
            df, posting_list=self.phrase_dict[phrase]
            for docID, tf in posting_list:
                w=1 + math.log10(tf)
                self.doc_lengths[docID] += w * w
            
        for docID in self.doc_lengths:
            self.doc_lengths[docID] = math.sqrt(self.doc_lengths[docID])
        
        logger.info("Computed doc lengths")

    def fuzzy_matcher(self, term: str, threshold=0.8)->str:
        """
        Utilise Jaro-Winkler similarity for fuzzy matching

        Args:
            term: Term to find matches for
            threshold: Minimum similarity threshold
        
        Returns:
            str: Best matching term from dictionary, or original term if no matches
        """
        if term in self.dictionary:
            return term

        best_match=term
        best_similarity=0

        dict_terms=list(self.dictionary.keys())
        
        for dict_term in dict_terms:
            similarity=jellyfish.jaro_winkler_similarity(term, dict_term)
            if similarity>=threshold and similarity>best_similarity:
                best_similarity=similarity
                best_match=dict_term
        
        #Fallback to soundex if no good fuzzy match
        if best_match==term:
            soundex_code=jellyfish.soundex(term)
            if soundex_code in self.soundex_dict:
                best_match=self.soundex_dict[soundex_code][0]
        
        if best_match!=term:
            logger.info(f"Fuzzy matched '{term}' to '{best_match}' (similarity: {best_similarity:.3f})")
            
        return best_match
    
    def build_tfidf_matrix(self, doc_texts_list):
        """
        Tokenizes and preprocesses each document again.

        Args:
            doc_texts_list: list of texts retrieved from each document
        
        Returns:
            nparray: tfidf weights
        """
        all_docs_tokens= [self.preprocess(text) for text in doc_texts_list]
        vocab= {}
        df= Counter()

        for tokens in all_docs_tokens:
            unique_terms= set(tokens)
            for t in unique_terms:
                df[t]+= 1
            for t in tokens:
                if t not in vocab:
                    vocab[t]= len(vocab)

        N= len(all_docs_tokens)
        vocab_size= len(vocab)
        tfidf= np.zeros((N, vocab_size))

        for i, tokens in enumerate(all_docs_tokens):
            tf= Counter(tokens)
            for term, freq in tf.items():
                if term in vocab:
                    j= vocab[term]
                    tf_weight= 1 + math.log10(freq)
                    idf= math.log10(N / (1 + df[term]))
                    tfidf[i, j]= tf_weight * idf
        return tfidf

    def kmeans(self, X, k, max_iter=100):
        """
        Basic reimplementation of K-means from scikit-learn
        Args:
            X: list of points
            k: number of clusters to be selected
            max_iter: number of cycles over which clusters to be chosen
        
        Returns:
            nparray (labels): 1D array of cluster labels
            nparray (centroids): 2D array of cluster centers of shape (k, num_features)
        """
        np.random.seed(42)
        idx= np.random.choice(len(X), k, replace=False)
        centroids= X[idx]

        for _ in range(max_iter):
            distances= np.linalg.norm(X[:, None] - centroids[None, :], axis=2)
            labels= np.argmin(distances, axis=1)

            new_centroids= np.array([
                X[labels== i].mean(axis=0) if np.any(labels==i) else centroids[i]
                for i in range(k)
            ])

            if np.allclose(new_centroids, centroids):
                break
            centroids= new_centroids

        return labels, centroids


    def perform_clustering(self)->None:
        """
        Perform k means clustering on the documents using tf-idf weights
        """
        if self.N<self.n_clusters:
            logger.info("insufficient documents")
            return
            
        logger.info(f"Performing K-means clustering with {self.n_clusters} clusters...")
        
        doc_texts_list = [self.doc_texts[doc_id] for doc_id in self.doc_texts]
        doc_ids = list(self.doc_texts.keys())

        tfidf_matrix = self.build_tfidf_matrix(doc_texts_list)
        cluster_labels, centroids = self.kmeans(tfidf_matrix, self.n_clusters)

        for i, doc_id in enumerate(doc_ids):
            self.doc_clusters[doc_id] = int(cluster_labels[i])
        for i in range(self.n_clusters):
            self.cluster_centers[i] = centroids[i]

        logger.info("Clustering completed.")


    def build_index(self) -> None:
        """
        Building VSM index for search
        """
        self.N=0
        postings=defaultdict(list)  # term --> list of (document id, term frequency)
        phrase_postings=defaultdict(list)  # phrase --> list of (document id, term frequency)

        for docID, filename in enumerate(os.listdir(self.corpus_dir), start=1):
            filepath=os.path.join(self.corpus_dir, filename)

            # text extraction
            engine=TextExtractionEngine(filepath)
            text=engine.run()
            if not text.strip():
                logger.warning(f"No text extracted from file: {filename}. Skipping this file.")
                continue
            
            # Clustering original text
            self.doc_texts[docID]=text
            
            # Preprocessing
            terms=self.preprocess(text)
            
            #docID file name mapping
            self.doc_index[docID]=filename

            #term freq count
            tf=Counter(terms)
            
            for term, frequency in tf.items():
                if "_" in term: #indicates phrase
                    phrase_postings[term].append((docID, frequency))
                else:
                    postings[term].append((docID, frequency))
            
            self.N+=1
        
        # Building term, soundex and phrase dictionaries
        for term, posting_list in postings.items():
            self.dictionary[term]=(len(posting_list), posting_list)
        
        for phrase, posting_list in phrase_postings.items():
            self.phrase_dict[phrase]=(len(posting_list), posting_list)
        
        for term in self.dictionary:
            soundex_code = jellyfish.soundex(term)
            self.soundex_dict[soundex_code].append(term)

        self.compute_doc_length()
        
        #performing k means clustering
        self.perform_clustering()
        
        logger.info(f"Built enhanced index for {self.N} documents with {len(self.dictionary)} terms and {len(self.phrase_dict)} phrases")
    
    def get_relevant_clusters(self, query_terms: list, top_clusters=3) -> list:
        """
        Identify relevant clusters
        
        Args:
            query_terms: Preprocessed query terms
            top_clusters: Number of top clusters to consider
            
        Returns:
            list: List of relevant cluster via cluster ID
        """
        if not self.doc_clusters:
            return list(range(self.n_clusters)) if self.n_clusters else []
        
        cluster_scores=defaultdict(float)
        
        for term in query_terms:
            matched_term=self.fuzzy_matcher(term)
            if matched_term in self.dictionary:
                df, posting_list=self.dictionary[matched_term]
                for docID, tf in posting_list:
                    if docID in self.doc_clusters:
                        cluster_id=self.doc_clusters[docID]
                        cluster_scores[cluster_id]+=math.log10(1 + tf)
        
        sorted_clusters=sorted(cluster_scores.items(), key=lambda x: x[1], reverse=True)
        relevant_clusters=[cluster_id for cluster_id, score in sorted_clusters[:top_clusters]]

        if not relevant_clusters:
            relevant_clusters=list(range(self.n_clusters))
        
        logger.info(f"Selected clusters {relevant_clusters} for query processing")
        return relevant_clusters

    def query(self, qtext):
        """
        Query processing with fuzzy matching, clustering, and phrase support
        
        Args:
            qtext: Query text
        
        Returns:
            tuple: (list of (filename, score) pairs, elapsed_time)
        """
        start_time=time.perf_counter()

        qtokens=self.preprocess(qtext)
        qtf=Counter(qtokens)

        qvec={}

        for term, tf in qtf.items():
            matched_term=term
            #matched phrases
            if "_" in term and term in self.phrase_dict:
                df, _=self.phrase_dict[term]
                dict_to_use=self.phrase_dict
            
            #match regular terms
            elif term in self.dictionary:
                df, _=self.dictionary[term]
                dict_to_use=self.dictionary

            else:
                #Apply fuzzy matching and fallback soundex processing
                matched_term=self.fuzzy_matcher(term)
                if matched_term!=term and matched_term in self.dictionary:
                    df, _=self.dictionary[matched_term]
                    dict_to_use=self.dictionary
                else:
                    continue
            
            if matched_term in dict_to_use:
                df, _=dict_to_use[matched_term]
                w=(1 + math.log10(tf)) * math.log10(self.N / df)
                qvec[matched_term]=w

        #normalize query vector as per provided requirements
        norm=math.sqrt(sum(w * w for w in qvec.values()))
        if norm>0:
            for term in qvec:
                qvec[term]/=norm

        relevant_clusters=self.get_relevant_clusters(list(qtf.keys()))

        scores=defaultdict(float)
        
        for term, qw in qvec.items():
            # Check both regular dictionary and phrase dictionary
            dict_to_use=self.dictionary if term in self.dictionary else self.phrase_dict
            
            if term in dict_to_use:
                df, posting_list=dict_to_use[term]
                for docID, tf in posting_list:
                    if (docID in self.doc_clusters and self.doc_clusters[docID] not in relevant_clusters):
                        continue
                    
                    dw=1 + math.log10(tf)
                    if docID in self.doc_lengths and self.doc_lengths[docID] > 0:
                        dw/=self.doc_lengths[docID]
                        scores[docID]+=qw * dw

        ranked=sorted(scores.items(), key=lambda x: (-x[1], x[0]))
        end_time=time.perf_counter()
        elapsed=end_time - start_time
        
        result_info=f"Processed enhanced query in {elapsed:.6f} seconds. Found {len(ranked)} matching documents"
        result_info+=f" across {len(relevant_clusters)} clusters"
        
        logger.info(result_info)
        
        return [(self.doc_index[docID], score) for docID, score in ranked[:10]], elapsed