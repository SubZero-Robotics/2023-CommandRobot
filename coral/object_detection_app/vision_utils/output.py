class Output:
    def __init__(self, bbox: list[float], classId: int, score: float):
        self.classId = classId
        self.score = score
        self.bbox = bbox

    def flatten(self) -> list[float]:
        vals: list[float] = []
        vals.append(float(self.classId), self.score)
        for coord in self.bbox:
            vals.append(coord)
        return vals
