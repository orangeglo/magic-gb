import sys
import os
import re
 
file_size = os.path.getsize(sys.argv[1])
final_line = next(reversed(list(open(sys.argv[1], 'rb'))))
final_line = re.sub(b"[^\xff]", b'', final_line)
used_bytes = file_size - len(final_line)
percentage = round(used_bytes / file_size * 100, 1)

print('### Usage: ' + str(percentage) + '% (' + str(used_bytes) + ' of ' + str(file_size) + ')')