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

logger=get_logger(__name__)
nltk.download("stopwords")

class VSM:
    """
    Implementation of vector space model for documents, on a directory basis
    """
    def __init__(self, corpus_dir: str)->None:
        """
        Initialises VSM class

        Args:
            corpus_dir: Directory containing files to be processed
        
        Returns:
            None
        """
        self.corpus_dir = corpus_dir

        self.doc_index={} # document id --> file name mapping
        self.dictionary={} # term --> (document frequency, postings list(document id, term frequency))
        self.doc_lengths={} # document id --> vector length
        self.soundex_dict=defaultdict(list)
        self.N=0 # number of documents

        #preprocessing tools
        self.stop_words=set(stopwords.words("english"))
        self.stemmer=PorterStemmer()
        logger.info(f"Initialized VSM for corpus directory: {corpus_dir}")
    
    def preprocess(self, text: str)->list:
        """
        Preprocesses the text by removing stop words, stemming using Porter's algorithms, and returning tokens
        
        Args:
            text: Text to be preprocessed
        
        Returns:
            list: List of tokens after preprocessing
        """
        text=text.lower()
        text=text.replace("-", " ").replace("_", " ")
        text=text.translate(str.maketrans("", "", string.punctuation))
        tokens=text.split()

        tokens=[
            self.stemmer.stem(t) for t in tokens if t not in self.stop_words and not t.isdigit()
        ]
        logger.info(f"Preprocessed text into {len(tokens)} tokens")
        return tokens

    def compute_doc_length(self):
        """
        Computes the vector length for each document in the corpus
        """
        self.doc_lengths=defaultdict(float)
        for term, (df, posting_list) in self.dictionary.items():
            for docID, tf in posting_list:
                w=1+math.log10(tf)
                self.doc_lengths[docID]+=w*w
            
        for docID in self.doc_lengths:
            self.doc_lengths[docID]=math.sqrt(self.doc_lengths[docID])
        logger.info("Computed document lengths for all documents")

    def build_index(self)->None:
        """
        Builds the inverted index for all documents in selected directory
        """
        self.N=0
        postings=defaultdict(list) # term --> list of (document id, term frequency), temporary dictionary

        for docID, filename in enumerate(os.listdir(self.corpus_dir), start=1):
            filepath=os.path.join(self.corpus_dir, filename)

            #text extraction
            engine=TextExtractionEngine(filepath)
            text=engine.run()
            if not text.strip():
                logger.warning(f"No text extracted from file: {filename}. Skipping this file.")
                continue
            
            #text preprocessing
            tokens=self.preprocess(text)

            #mapping docID to filename
            self.doc_index[docID]=filename

            #couting term freq
            tf=Counter(tokens)
            for term, frequency in tf.items():
                postings[term].append((docID, frequency))
            
            self.N+=1
        
        #building dictionary
        for term, posting_list in postings.items():
            self.dictionary[term]=(len(posting_list), posting_list)
        
        #building soundex dictionary
        for term in self.dictionary:
            soundex_code=jellyfish.soundex(term)
            self.soundex_dict[soundex_code].append(term)

        self.compute_doc_length()
        logger.info(f"Built inverted index for {self.N} documents with {len(self.dictionary)} unique terms")
    
    def query(self, qtext):
        """
        Handles processing query and returns top 10 or less list of relevant documents

        Args:
            qtext: Query text
        
        Returns:
            list: List of top 10 or less relevant documents
            
            float: Time taken to process the query
        """
        start_time=time.perf_counter() #timer start

        qtokens=self.preprocess(qtext)
        qtf=Counter(qtokens)

        qvec={}
        for term, tf in qtf.items():
            if term not in self.dictionary:
                soundex_code=jellyfish.soundex(term)
                if soundex_code in self.soundex_dict:
                    term=self.soundex_dict[soundex_code][0]
                else:
                    continue
            
            #generating query vector
            df, _=self.dictionary[term]
            w=(1+math.log10(tf)) * math.log10(self.N/df)
            qvec[term]=w
        
        #normalising query vector
        norm=math.sqrt(sum(w*w for w in qvec.values()))
        if norm>0:
            for term in qvec:
                qvec[term]/=norm
        
        #computing score
        scores=defaultdict(float)   
        for term, qw in qvec.items():
            df, posting_list=self.dictionary[term]
            for docID, tf in posting_list:
                dw=1+math.log10(tf)
                dw/=self.doc_lengths[docID]
                scores[docID]+=qw*dw
        
        ranked=sorted(scores.items(), key=lambda x: (-x[1], x[0]))
        end_time=time.perf_counter() #timer end
        elapsed=end_time-start_time
        logger.info(f"Processed query in {end_time-start_time:.6f} seconds. Found {len(ranked)} matching documents.")
        return [(self.doc_index[docID], score) for docID, score in ranked[:10]], elapsed




