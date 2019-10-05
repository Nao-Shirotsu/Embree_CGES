#include "util_objfile.hpp"

#include "constants.hpp"

namespace cges::obj {

NumElements::NumElements()
    : m_count{ 0, 0, 0, 0, 0, 0 } {}

size_t& NumElements::operator[](const Marker idx) {
  return m_count[static_cast<int>(idx)];
}

Marker ToMarker(const std::string& marker) {
  if (marker == "v") {
    return Marker::V;
  }
  if (marker == "f") {
    return Marker::F;
  }
  if (marker == "vt") {
    return Marker::VT;
  }
  if (marker == "vn") {
    return Marker::VN;
  }
  if (marker == "g") {
    return Marker::G;
  }
  if (marker == "mtllib") {
    return Marker::MTLLIB;
  }
  if (marker == "usemtl") {
    return Marker::USEMTL;
  }
  return Marker::SKIP;
}

bool LoadFileProperties(std::ifstream& ifs, obj::NumElements& count, bool fTriple) {
  char dummy[INPUT_BUFFER_SIZE];
  bool fTripleWritten = false;
  std::string markerStr;
  while (!ifs.eof()) {
    if (ifs.peek() == '#') {
      ifs.getline(dummy, INPUT_BUFFER_SIZE);
      continue;
    }
    ifs >> markerStr;
    const auto marker = obj::ToMarker(markerStr);
    if (marker == Marker::F && !fTripleWritten) {
      ifs >> markerStr;
      const auto pos = markerStr.find_first_of("/");
      fTriple = (pos == std::string::npos ? false : true);
      fTripleWritten = true;
    }
    ifs.getline(dummy, INPUT_BUFFER_SIZE);
    if(marker == Marker::SKIP) {
      continue;
    }
    ++count[marker];
  }
  ifs.clear();
  ifs.seekg(0, std::ios::beg);
  return bool(ifs);
}

}