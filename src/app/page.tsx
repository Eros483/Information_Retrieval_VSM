// import InformationRetrievalApp from "@/components/InformationRetrievalApp"

// export default function Home() {
//   return <InformationRetrievalApp />
// }
'use client';

import React from "react";
import FloatingSearch from "@/components/FloatingSearch";

const TestFloating = () => {
  return (
    <div className="min-h-screen bg-gray-800 flex items-center justify-center text-white">
      <h1 className="absolute top-4 left-4 text-2xl">Testing Floating Search</h1>
      <FloatingSearch />
    </div>
  );
};

export default TestFloating;
