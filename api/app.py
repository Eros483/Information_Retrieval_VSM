from fastapi import FastAPI, Query
from pydantic import BaseModel
from typing import List, Tuple, Optional
import uvicorn
import os

from src.vsm_basic import VSM
from utils.logger import get_logger

logger = get_logger(__name__)

app = FastAPI(
    title="Vector Space Model Search API",
    description="Search documents using lnc.ltc vector space model with Soundex fallback",
    version="1.0.0"
)

#globally declaring VSM engine
vsm_engine: Optional[VSM]=None
current_corpus: Optional[str]=None

class QueryResponse(BaseModel):
    query: str
    results: List[Tuple[str, float]]
    elapsed_time: float

@app.get("/")
def root():
    return {"message": "Welcome to the Vector Space Model Search API. Use /build to set the document corpus and /search to perform searches."}

@app.post("/build")
def build_index(corpus_dir: str = Query(..., description="Path to the directory containing documents")):
    """
    Build the VSM index for the given corpus directory
    Args:
        corpus_dir: Path to the directory containing documents
    Returns:
        dict: Message indicating success or failure
    """
    global vsm_engine, current_corpus

    if not os.path.isdir(corpus_dir):
        logger.error(f"Invalid corpus directory: {corpus_dir}")
        return {"error": "Invalid directory path"}
    
    vsm_engine = VSM(corpus_dir)
    vsm_engine.build_index()
    current_corpus = corpus_dir

    logger.info(f"Built index for corpus directory: {corpus_dir}")
    return {"message": f"Index built for corpus directory: {corpus_dir}"}

@app.get("/search", response_model=QueryResponse)
def search(query: str = Query(..., description="Search query string")):
    """
    Search the indexed VSM space for the given query

    Args:
        query: The search query string

    Returns:
        QueryResponse: Contains the original query, list of (document, score) tuples, and time taken to search
    """
    if vsm_engine is None:
        logger.error("Search attempted before building index")
        return {"error": "Index not built. Please build the index using /build endpoint."}
    
    results, elapsed_time = vsm_engine.query(query)
    logger.info(f"Search completed for query: '{query}' with {len(results)} results in {elapsed_time:.6f} seconds")
    
    return QueryResponse(query=query, results=results, elapsed_time=elapsed_time)

if __name__=="__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)