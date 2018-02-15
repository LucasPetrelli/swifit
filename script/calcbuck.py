vsat = 1.5
vf = 0.3

vin = 6
vout = 3.3
Iout = 1
fmin = 300e3
vr = 0.01

trat = (vout + vf)/(vin - vsat - vout)
tsum = 1/fmin
toff = tsum/(trat+1)
ton = tsum-toff
ct = 4e-5 * ton
ipk = 2*Iout
rsc = 0.3/ipk
Lmin = ((vin - vsat - vout)/ipk)*ton
co = (ipk*tsum)/(8*vr)
rrat = vout/1.25-1

r1 = 0.68e3
r2 = rrat*r1
print(r2)

print('Ct={ct}\nRsc = {rsc}\nL(min) = {Lmin}\nCo = {co}\nR1/R2 = {rrat}'.format(**locals()))