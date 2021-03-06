% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/variantkey.R
\name{AreOverlappingRegionRegionKey}
\alias{AreOverlappingRegionRegionKey}
\title{Check if a region and a regionkey are overlapping.
Return 1 if the regions overlap, 0 otherwise.}
\usage{
AreOverlappingRegionRegionKey(chrom, startpos, endpos, rk)
}
\arguments{
\item{chrom}{Region A chromosome code.}

\item{startpos}{Region A start position.}

\item{endpos}{Region A end position (startpos + region length).}

\item{rk}{RegionKey or region B.}
}
\description{
Check if a region and a regionkey are overlapping.
Return 1 if the regions overlap, 0 otherwise.
}
