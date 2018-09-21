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
db.commit()

cursor.execute('''INSERT OR IGNORE INTO runers(place, number, name, groups, address, comment, 
                half_way, one_min, result_h, result_sensor) VALUES(?,?,?,?,?,?,?,?,?,?)''',
                (1,2,"3","4","5","6",7,8,9,10))

db.commit()
db.close()


def parser_float(in_str=""):

    result=0.0
    m = re.search('(\d{2}:\d{2}:\d{2})|(\d{1}:\d{2}:\d{2})|(\d{2}:\d{2})', line.split("\t")[8])
    if m.group(0) != None:
        if len(m.group(0).split(":")) == 2:
            value = float(m.group(0).split(":")[0]) + float(m.group(0).split(":")[1]) / 60
            data_all.append([line.split("\t")[3], value])
        if len(m.group(0).split(":")) == 3:
            if float(m.group(0).split(":")[0]) < 10:
                value = float(m.group(0).split(":")[0]) * 60 + float(m.group(0).split(":")[1]) + float(
                    m.group(0).split(":")[2]) / 60
                data_all.append([line.split("\t")[3], value])
            if float(m.group(0).split(":")[0]) > 9:
                value = float(m.group(0).split(":")[0]) + float(m.group(0).split(":")[1]) / 60
                data_all.append([line.split("\t")[3], value])