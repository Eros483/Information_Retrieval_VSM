import { SearchResult } from "@/types/search";

const API_BASE = "http://localhost:8000";

export const buildIndex = async (directoryPath: string) => {
  const response = await fetch(`${API_BASE}/build?corpus_dir=${encodeURIComponent(directoryPath)}`, {
    method: "POST",
    headers: { "Accept": "application/json" }
  });

  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return response.json();
};

export const performSearch = async (query: string): Promise<SearchResult> => {
  const response = await fetch(`${API_BASE}/search?query=${encodeURIComponent(query)}`, {
    method: "GET",
    headers: { "Accept": "application/json" }
  });

  if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
  return response.json();
};
