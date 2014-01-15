import re

f = open("src/regal/RegalState.h", "r" )
count = 0
pre = re.compile( ".*\.gl([A-Za-z0-9_]+).*" );
calls = []
for line in f:
  count += 1
  m = pre.match( line )
  if m and len(m.groups()):
    if m.group(1) not in calls:
      calls.append( m.group(1) )

for c in sorted( calls ):
  print '  \'gl%s\',' % c


