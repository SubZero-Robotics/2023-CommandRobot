class Output:
    def __init__(self, bbox: list[float], classId: int, score: float):
        self.classId = classId
        self.score = score
        self.bbox = bbox

    def flatten(self) -> list[float]:
        vals: list[float] = []
        # TODO: Covert the Output object into a single list of floats
        return vals