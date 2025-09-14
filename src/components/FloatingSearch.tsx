'use client';

import { useState, useEffect } from "react";
import { buildIndex, performSearch } from "@/services/api";
import SearchResults, { SearchResult } from "@/components/SearchResults";
import FloatingInput from "./FloatingInput";

const FloatingSearch = () => {
  const [visible, setVisible] = useState(false);
  const [step, setStep] = useState<"dir" | "query">("dir");
  const [dir, setDir] = useState("");
  const [query, setQuery] = useState("");
  const [results, setResults] = useState<SearchResult | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [selectedIndex, setSelectedIndex] = useState<number>(-1);

  useEffect(() => {
    setDir(localStorage.getItem("lastDir") || "");
  }, []);

  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      if (e.ctrlKey && e.code === "Space") {
        e.preventDefault();
        setVisible((v) => !v);
        setStep("dir");
        setSelectedIndex(-1);
        setQuery("");
        setResults(null);
      }
      if (e.key === "Escape") setVisible(false);

      if (visible && step === "query" && results?.results.length) {
        if (e.key === "ArrowDown") {
          e.preventDefault();
          setSelectedIndex((prev) => (prev + 1) % results.results.length);
        }
        if (e.key === "ArrowUp") {
          e.preventDefault();
          setSelectedIndex((prev) =>
            prev <= 0 ? results.results.length - 1 : prev - 1
          );
        }
      }
    };
    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [visible, step, results]);

  useEffect(() => {
    const handleEnter = (e: KeyboardEvent) => {
      if (e.key === "Enter" && visible) {
        if (step === "dir") handleDirSubmit();
        if (step === "query" && selectedIndex >= 0 && results?.results.length) {
          e.preventDefault();
          console.log("Selected:", results.results[selectedIndex][0]);
        }
      }
    };
    window.addEventListener("keydown", handleEnter);
    return () => window.removeEventListener("keydown", handleEnter);
  }, [visible, step, selectedIndex, results]);

  const handleDirSubmit = async () => {
    if (!dir.trim()) return;
    localStorage.setItem("lastDir", dir);
    setLoading(true);
    setError(null);
    try {
      await buildIndex(dir);
      setStep("query");
    } catch (err) {
      setError(err instanceof Error ? err.message : "Failed to build index");
    } finally {
      setLoading(false);
    }
  };

const handleQuerySubmit = async () => {
  if (!query.trim()) {
    setResults(null); 
    return;
  }
  setLoading(true);
  setError(null);
  try {
    const res: SearchResult = await performSearch(query);
    setResults(res);
    setSelectedIndex(0);
  } catch (err) {
    setError(err instanceof Error ? err.message : "Search failed");
    setResults(null);
  } finally {
    setLoading(false);
  }
};

useEffect(() => {
  if (step === "query") {
    setResults(null); 
  }
}, [query]);

  if (!visible) return null;

  return (
    <>
      <div className="fixed inset-0 bg-black/40 backdrop-blur-sm z-40" />
      <div className="fixed inset-0 flex flex-col items-center justify-start pt-32 gap-4 z-50 w-full px-4">
        {step === "dir" && (
          <FloatingInput
            value={dir}
            onChange={setDir}
            onSubmit={handleDirSubmit}
            placeholder="Enter directory path..."
            autoFocus
          />
        )}

        {step === "query" && (
          <>
            <FloatingInput
              value={query}
              onChange={setQuery}
              onSubmit={handleQuerySubmit}
              placeholder="Search query..."
              autoFocus
            />
            {loading && (
              <div className="text-gray-400 text-sm mt-2">Loading...</div>
            )}
            {error && (
              <div className="text-red-400 text-sm mt-2">{error}</div>
            )}
            {/* Search Results */}
            {results && (
              <SearchResults
                results={results}
                selectedIndex={selectedIndex}
              />
            )}
          </>
        )}
      </div>
    </>
  );
};

export default FloatingSearch;
