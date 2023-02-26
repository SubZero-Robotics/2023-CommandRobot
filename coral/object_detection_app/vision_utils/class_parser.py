import csv
import operator
from typing import Tuple, List


class DetectionModelClass(object):
    def __init__(self, id: int, name: str, color: Tuple[int]):
        self.id = id
        self.name = name
        self.color = color

    def __lt__(self, other) -> bool:
        return self.id < other.id


class DetectionModelClassParser:
    def parse(filename: str) -> List[DetectionModelClass]:
        with open(filename, mode='r') as file:
            csvReader = csv.DictReader(file, delimiter='|', fieldnames=[
                                       'id', 'name', 'color'])
            detectionModelClasses: List[DetectionModelClass] = []
            for row in csvReader:
                color = [int(i) for i in row['color'].split(',')]
                print('row', row)
                detectionModelClasses.append(DetectionModelClass(int(row['id']), row['name'],
                                                                 color))
        # Ensure we order them correctly in the list by id
        detectionModelClasses.sort(key=operator.attrgetter('id'))
        return detectionModelClasses
