export interface SearchResult {
  query: string;
  results: [string, number][];
  elapsed_time: number;
}
