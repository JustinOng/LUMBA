def getPixelIndex(i, start, end):
    if start < end:
        return start + i
    else:
        return end - i

def withinSegment(i, start, end):
    if start < end:
        return (start + i) <= end
    else:
        return (start - i) >= end

print(getPixelIndex(0, 10, 20))
print(getPixelIndex(5, 10, 20))
print(getPixelIndex(0, 20, 10))
print(getPixelIndex(5, 20, 10))

print(withinSegment(0, 10, 20))
print(withinSegment(10, 10, 20))
print(withinSegment(11, 10, 20))

print(withinSegment(0, 20, 10))
print(withinSegment(10, 20, 10))
print(withinSegment(11, 20, 10))
