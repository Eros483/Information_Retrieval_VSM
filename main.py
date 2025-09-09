from src.vsm_basic import VSM
from utils.logger import get_logger
from pathlib import Path

logger=get_logger(__name__)

if __name__=="__main__":
    corpus_path = Path("/mnt/d/collegeBooks/sem4/INT205/feb")
    vsm=VSM(corpus_path)
    vsm.build_index()

    query=input("Enter your query: ")
    results=vsm.query(query)
    for arr in results:
        print(f"Document: {arr[0]}, Score: {arr[1]}")
    
    if not results:
        print("No documents matched the query.")

    logger.info("Vector Space Model processing completed.")