import csv
import operator


class DetectionModelClass(object):
    def __init__(self, id: int, name: str, color: tuple[int]):
        self.id = id
        self.name = name
        self.color = color
    
    def __lt__(self, other) -> bool:
        return self.id < other.id

class DetectionModelClassParser:
    def parse(filename: str) -> list[DetectionModelClass]:
        with open(filename, mode='r') as file:
            csvReader = csv.DictReader(file, delimiter='|', fieldnames=['id', 'name', 'color'])
            detectionModelClasses: list[DetectionModelClass]
            for row in csvReader:
                color = [int(i) for i in row['color'].split(',')]
                detectionModelClasses.append(DetectionModelClass(int(row['id']), row['name'],
                                                                 color))
        # Ensure we order them correctly in the list by id
        detectionModelClasses.sort(key=operator.attrgetter('id'))
        return detectionModelClasses