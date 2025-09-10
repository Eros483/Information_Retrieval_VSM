# <div align="center">Vector Space Model for Information Retrieval</div>
<div align="center">
  <em>Assignment 1 for CSD358 Information Retrieval</em>
</div>

## Provided Features
- Ranked retrieval utilising vector space indexing, and cosine similarity based search.
- Follows lnt.ltc ranking schemes.
- Supports `n-gram` searches.
- Supports fallback searches via `fuzzy` searches measuring `Jaro-Winkler similarity`, and `Soundex` algorithm.
- Utilises `K-means clustering` via tf-idf weights based cluster generation.
- Supports information retrieval from documents such as PDFs, TXTs, DOCX and OCR.