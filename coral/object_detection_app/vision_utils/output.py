from typing import List

class Output:
    def __init__(self, bbox: List[float], classId: int, score: float):
        self.classId = classId
        self.score = score
        self.bbox = bbox
        
    def __str__(self):
        return f"Output: class={self.classId}|score={self.score}|bbox={self.bbox}"

    def flatten(self) -> List[float]:
        vals: List[float] = []
        vals.append(float(self.classId), self.score)
        for coord in self.bbox:
            vals.append(coord)
        return vals
