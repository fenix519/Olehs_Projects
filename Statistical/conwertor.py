import re
import matplotlib.pyplot as plt
import numpy as np

file = open('running_data_base.txt',encoding="utf8")

print (file.readline())
print (file.readline().split("\t"))

data=[]

star_range = 32
stop_range = 80
wide = stop_range - star_range

runers=0
# selecting data from file
for line in file :
   runers+=1
   m = re.search('(\d+:\d+:\d+)|(\d+:\d+)', line.split("\t")[8] )
   if m.group(0) != None:
       if len(m.group(0).split(":")) == 2:
           value = float(m.group(0).split(":")[0])+ float(m.group(0).split(":")[1])/60
           data.append(value)
       if len(m.group(0).split(":")) == 3:
           value = float(m.group(0).split(":")[0])*60 + float(m.group(0).split(":")[1]) + float(m.group(0).split(":")[2])/60
           data.append(value)
file.close()



# calculation propability
frequency = []
parameter = list(range(star_range,stop_range))

for p in parameter :
   counter = 0
   for d in data :
      if (p <= d) and (d < p+1):
         counter +=1
   frequency.append(counter/runers*100)


for k in frequency:
   print(k)


#x = np.linspace(0, 10, 100)

# Plot the data
plt.plot(parameter, frequency, label='linear')

plt.xlabel('time (min)')
plt.ylabel('propability %')
plt.title('distribution of runers')
plt.grid(True)

# Add a legend
plt.legend()
# Show the plot
plt.show()



   


