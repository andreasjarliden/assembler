#pragma once

#include <memory>

class Segment;

class Segments {
public:
    Segments();
    ~Segments();

    void addSegment(size_t index);
    int numberOfSegments() const;
    const Segment& index(size_t index) const;
    Segment& index(size_t index);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

