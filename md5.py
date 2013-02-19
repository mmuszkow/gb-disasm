import hashlib
print hashlib.md5(open('red.gb', 'r').read()).hexdigest()