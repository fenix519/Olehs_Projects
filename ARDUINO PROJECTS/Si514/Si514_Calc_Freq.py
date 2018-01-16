#!/usr/bin/python
# Si514 register calculation
#Author Oleh Humenchuk 27.05.2016

import serial
import time
import io

def Calc_LS_DIV(Fout):
    LS_DIV = 2080.0/(Fout * 1022)
    buf=0
    for i in range(7):
        if((2**i <= LS_DIV) and (LS_DIV <= 2**(i+1))):
            buf = 2**(i+1)
    if(buf!=0):
        return(buf)
    else:
        return(1)

def Calc_HS_DIV(Fout):
    HS_DIV = 2080.0/(Fout * Calc_LS_DIV(Fout))
    for i in range(10,1022):
        if((i <= HS_DIV) and (HS_DIV <= (i+1))):
           return((i+1))

def Get_LP(M):
    M_Table=[[78.173858662,75.843265046,4,4],
             [75.843265046,72.937624981,3,4],
             [72.937624981,67.859763463,3,3],
             [67.859763463,65.259980246,2,3],
             [65.259980246,65.040650407,2,2]]
    if(max(col[0] for col in M_Table ) <  M) or (min(col[1] for col in M_Table ) >  M) :
        print("Error: M isn't in correct range !!!!")
    else:
        for i in range(len(M_Table)):
            if(M < M_Table[i][0]) and  (M>M_Table[i][1]):
                return [M_Table[i][2:4]]


def Calc_Config(Fout):

    RegFile=[]
    LS_DIV=Calc_LS_DIV(Fout)
    HS_DIV=Calc_HS_DIV(Fout)
    #HS_DIV=42 # corection
    M=LS_DIV*HS_DIV*Fout/Fxo

    M_int=int(M)
    M_Frac=int((M-M_int)*(2**29))

    B_M_Frac="0"*(31-(len(bin(M_Frac))))+bin(M_Frac)[2:];
    L_M_Frac=len(B_M_Frac);

    B_M_int="0"*(11-(len(bin(M_int))))+bin(M_int)[2:];
    L_M_int=len(B_M_int);

    LS_DIV-=1 # corection

    B_LS_DIV=bin(LS_DIV)[2:];
    L_LS_DIV=len(B_LS_DIV);

    B_HS_DIV="0"*(12-(len(bin(HS_DIV))))+bin(HS_DIV)[2:];
    L_HS_DIV=len(B_HS_DIV);

    RegFile.append(["0x0",hex(Get_LP(M)[0][0])+hex(Get_LP(M)[0][1])[2:]])
    RegFile.append(["0x5",hex(int(B_M_Frac[(L_M_Frac-8):(L_M_Frac)],2))])
    RegFile.append(["0x6",hex(int(B_M_Frac[(L_M_Frac-16):(L_M_Frac-8)],2))])
    RegFile.append(["0x7",hex(int(B_M_Frac[(L_M_Frac-24):(L_M_Frac-16)],2))])
    RegFile.append(["0x8",hex(int(B_M_int[(L_M_int-3):(L_M_int)]+ B_M_Frac[:(L_M_Frac-24)],2))])
    RegFile.append(["0x9",hex(int(B_M_int[:(L_M_int-3)],2))])
    RegFile.append(["0xA",hex(int(B_HS_DIV[(L_HS_DIV-8):(L_HS_DIV)],2))])
    RegFile.append(["0xB",hex(int(B_LS_DIV[(L_LS_DIV-3):],2))+   hex(int(B_HS_DIV[:(L_HS_DIV-8)],2))[2:]])

#    print("LS_DIV=",LS_DIV)
#    print("HS_DIV=",HS_DIV)
#    print("M=",M)
#    print("M_int=",M_int)
#    print("M_Frac=",M_Frac)

    return RegFile

Fout = 100
Fxo = 31.98

RegFile=Calc_Config(Fout)


#for i in RegFile:
#    print i

for i in range(len(RegFile)):
    print(int(RegFile[i][0],16))
    print(int(RegFile[i][1],16))


ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0.5)
ser.close()
ser.open()

print(ser)

ser.write(b"start")
time.sleep(1)
ser.close()