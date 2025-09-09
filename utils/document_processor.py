import tempfile
from pdf2image import convert_from_path
import pytesseract
from io import StringIO
import fitz
import docx
import os
from utils.logger import get_logger

logger=get_logger(__name__)

class TextExtractionEngine:
    """
    A class to handle text extractions.
    Supports:
    - PDF (text-based and image-based)
    - DOCX
    - TXT
    """
    def __init__(self, file_path: str)->None:
        """
        Initializes the TextExtractionEngine with the given file path.
        
        Args:
            file_path: Path to the file from which text needs to be extracted.
        
        Returns:
            None
        """
        self.file_path = file_path
        self.file_extension = os.path.splitext(file_path)[1].lower()
        logger.info(f"Initialized TextExtractionEngine for file: {file_path}")

    def read_img_pdfs(self, file_path):
        """
        Convert PDF to text using OCR
        
        Args:
            file_path: Path to the PDF file
        
        Returns:
            str: Extracted text from the PDF
        """
        
        # Create a temporary file to save the uploaded PDF
        with tempfile.NamedTemporaryFile(delete=False, suffix='.pdf') as tmp_file:
            tmp_file.write(file_path.read())
            tmp_file_path = tmp_file.name
        
        try:
            # Convert PDF pages to images
            pages = convert_from_path(tmp_file_path)
            
            # OCR each page and collect text
            full_text = []
            
            for i, page in enumerate(pages, start=1):         
                text = pytesseract.image_to_string(page)
                full_text.append(text)
            
            if full_text:
                logger.info(f"Extracted text from image-based PDF: {self.file_path}")

            return "\n".join(full_text)
        
        finally:
            # Clean up temporary file
            os.unlink(tmp_file_path)

    def read_docx(self, file_path):
        """
        Read text from a DOCX file
        
        Args:
            file_path: Path to the DOCX file
        
        Returns:
            str: Extracted text from the DOCX file
        """
        doc = docx.Document(file_path)
        full_text = []
        
        for para in doc.paragraphs:
            full_text.append(para.text)
        
        if full_text:
            logger.info(f"Extracted text from DOCX: {self.file_path}")

        return "\n".join(full_text)

    def read_pdf(self, file_path):
        """
        Read text from a PDF file
        
        Args:
            file_path: Path to the PDF file
        
        Returns:
            str: Extracted text from the PDF file
        """
        text = ""
        with fitz.open(file_path) as doc:
            for page in doc:
                text += page.get_text()
        if text:
            logger.info(f"Extracted text from PDF: {self.file_path}")
        return text

    def read_txt(self, file_path):
        with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
            full_text = f.read()
            if full_text:
                logger.info(f"Extracted text from TXT: {self.file_path}")
            return full_text

    def run(self):
        """
        Extract text based on file type

        Returns:
            str: Extracted text from the file
        """
        if self.file_extension == ".pdf":
            try:
                return self.read_pdf(self.file_path)
            except:
                return self.read_img_pdfs(self.file_path)
        
        elif self.file_extension == ".docx":
            return self.read_docx(self.file_path)

        elif self.file_extension == ".txt":
            return self.read_txt(self.file_path)
        
        else:
            logger.error(f"Unsupported file type: {self.file_extension} for file: {self.file_path}")
            raise ValueError("Unsupported file type")