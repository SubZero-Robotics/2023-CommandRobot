class Output:
    def __init__(self, bbox: list[float], classId: int, score: float):
        self.classId = classId
        self.score = score
        self.bbox = bbox

    def flatten(self) -> list[float]:
