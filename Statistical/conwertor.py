import re
import matplotlib.pyplot as plt
import numpy as np


star_range = 32
stop_range = 80

groups = ["Ч20", "Ч20-29", "Ч30-39", "Ч40-49", "Ч50-59", "Ч60-69", "Ч70+", "Ж20", "Ж20-29", "Ж30-39", "Ж40-49", "Ж50-69", "Ж60-69", "Ж70+"]

def prop_calculator(data):
   prop = []
   runners = 0
   for p in list(range(star_range,stop_range)):
      counter = 0
      for d in data :
         if (p <= d[1]) and (d[1] < p+1):
            counter +=1
      prop.append(counter)
   return prop

file = open('all.txt',encoding="utf8")

print(file.readline())
print(file.readline().split("\t"))


data_all = []
data_separate = [] 

runers=0
# selecting data from file
for line in file :
   runers+=1
   m = re.search('(\d{2}:\d{2}:\d{2})|(\d{1}:\d{2}:\d{2})|(\d{2}:\d{2})', line.split("\t")[8] )
   if m.group(0) != None:
       if len(m.group(0).split(":")) == 2:
           value = float(m.group(0).split(":")[0])+ float(m.group(0).split(":")[1])/60
           data_all.append([line.split("\t")[3],value])
       if len(m.group(0).split(":")) == 3:
           if float(m.group(0).split(":")[0]) < 10 :
              value = float(m.group(0).split(":")[0])*60 + float(m.group(0).split(":")[1]) + float(m.group(0).split(":")[2])/60
              data_all.append([line.split("\t")[3],value])
           if float(m.group(0).split(":")[0]) > 9 :
              value = float(m.group(0).split(":")[0]) + float(m.group(0).split(":")[1])/60 
              data_all.append([line.split("\t")[3],value])


file.close()

# calculation propability


#x = np.linspace(0, 10, 100)
# Plot the data
plt.plot(list(range(star_range,stop_range)), prop_calculator(data_all), label='linear')

plt.xlabel('time (min)')
plt.ylabel('propability %')
plt.title('distribution of runers')
plt.grid(True)

# Add a legend
plt.legend()
# Show the plot
plt.show()



   


