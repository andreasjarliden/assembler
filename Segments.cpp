#include "Segments.hpp"
#include "Segment.hpp"
#include <vector>

struct Segments::Impl {
  std::vector<Segment> segments;
};

Segments::Segments()
  : _impl(new Impl) {
  _impl->segments.push_back(Segment());
}

Segments::~Segments() {}

void Segments::addSegment() {
  _impl->segments.push_back(Segment());
}

int Segments::numberOfSegments() const {
  return _impl->segments.size();
}

const Segment& Segments::index(size_t i) const {
  return _impl->segments.at(i);
}

Segment& Segments::index(size_t i) {
  return _impl->segments.at(i);
}
