class Output:
    def __init__(self, bbox: float, classId: int, score: float):
        self.classId = classId
        self.score = score
        self.bbox = bbox
