import sqlite3
import re

db = sqlite3.connect('runer_2018_lviv.db')
cursor = db.cursor()

# cursor = db.cursor()
# cursor.execute('''DROP TABLE runers''')
# db.commit()

# cursor.execute('''
#                  CREATE TABLE runers( place INTEGER PRIMARY KEY, number INTEGER, name TEXT,
#                 groups TEXT, address TEXT, comment TEXT, half_way REAL, one_min REAL,
#                 result_h REAL, result_sensor REAL )''')


def parser(in_str=""):
    result=0.0
    m = re.search('(\d+:\d+:\d+)|(\d+:\d+)', in_str) # time parser
    if m.group(0) != None:
        if len(m.group(0).split(":")) == 2:
            result = float(m.group(0).split(":")[0]) + float(m.group(0).split(":")[1]) / 60
        if len(m.group(0).split(":")) == 3:
            result = float(m.group(0).split(":")[0]) * 60 + float(m.group(0).split(":")[1]) + float(
                m.group(0).split(":")[2]) / 60
    return(round(result,2))


print(parser("1:14:25"))
print(parser("3:18"))
print(parser("32:23"))


file = open('all.txt',encoding="utf8") #reading data log
print(file.readline())
print(file.readline().split("\t"))

for line in file:
    print(".")
    line.split("\t")[0]

    #cursor.execute('''INSERT OR IGNORE INTO runers(place, number, name, groups, address, comment,
    #                    half_way, one_min, result_h, result_sensor) VALUES(?,?,?,?,?,?,?,?,?,?)'''

    cursor.execute('''INSERT OR IGNORE INTO runers(place, number, name, groups, address, comment, 
                    half_way, one_min, result_h, result_sensor) VALUES(?,?,?,?,?,?,?,?,?,?)''',
                   (int(line.split("\t")[0]),
                    2, "3", "4", "5", "6", 7, 8, 9, 10))


db.commit()
db.close()
file.close()
