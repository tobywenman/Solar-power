import csv

class day:
    year = 0
    month = 0
    day = 0
    irradiance = 0

    def __init__(self,year,month,day):
        self.year = year
        self.month = month
        self.day = day
    
    def add_irradiance(self,irradiance):
        self.irradiance += irradiance

    def dateMatch(self, date):
        if int(date[0]) == self.year and int(date[1]) == self.month and int(date[2]) == self.day:
            return True
        else:
            return False
    
    def __str__(self):
        return str(self.year) + " " + str(self.month) + " " + str(self.day) + " " + str(self.irradiance)

with open("irradiance.csv", newline="") as csvFile:
    reader = csv.reader(csvFile)
    next(reader)
    days = []
    for row in reader:
        date = row[0].split("-")
        date[2] = date[2][0:2]
        if len(days) != 0 and days[-1].dateMatch(date):
            days[-1].add_irradiance(int(row[3]))
        else:
            days.append(day(int(date[0]),int(date[1]),int(date[2])))
            days[-1].add_irradiance(int(row[3]))

orderedDays = []
orderedDays.append(days[0])
days = days[1:]


for j in days:
    index = 0
    for i in orderedDays:
        if i.irradiance < j.irradiance:
            orderedDays.insert(index,j)
            break
        index += 1

with open("out.csv","w") as outcsv:
    writer = csv.writer(outcsv)

    for i in orderedDays:
        writer.writerow([i.year,i.month,i.day,i.irradiance])

