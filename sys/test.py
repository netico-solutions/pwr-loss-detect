 
import pwrloss

print('Test random number: {}\n'.format(pwrloss.dummy_number()))

power_int = pwrloss.pwrloss()
if power_int == 1:
    power_str = 'OK'
else:
    power_str = 'MISSING'
print('Power: {}! (code {})'.format(power_str, power_int))
