

local grd = require "wetgenes.grd"


local function process(name)

local g=grd.create( "art/"..name..".png" )
local fp=io.open("art/"..name..".c","wb")

local cmap=g:palette(0,256)
local bmap=g:pixels(0,0,0,g.width,g.height,1)

fp:write("// This file is machine generated and should not be edited...\n")
fp:write("\n")

fp:write(string.format("const int width=%d;\n",g.width))
fp:write(string.format("const int height=%d;\n",g.height))
fp:write("\n")


fp:write("static const unsigned int cmap[]={\n")
for idy=0,240,16 do
	local cc=","
	if idy==240 then cc="" end
	local ss={}
	for idx=0,15 do
		local r=cmap[(idx+idy)*4+1]
		local g=cmap[(idx+idy)*4+2]
		local b=cmap[(idx+idy)*4+3]
		local a=cmap[(idx+idy)*4+4]
		if a>64 then -- solid
			ss[#ss+1]=string.format("0x%06X",r*65536 + g*256 + b)
		else
			ss[#ss+1]="-1"
		end
	end
	fp:write(table.concat(ss,","))
	fp:write(cc.."\n")
end
fp:write("};\n")

fp:write("static const unsigned char bmap[]={\n")

local idx=1
for y=1,g.height do

	local cc=","
	if y==g.height then cc="" end

	local ss={}

	for x=1,g.width do

		ss[#ss+1]=string.format("0x%02X",bmap[idx])
		idx=idx+1
	end
	
	fp:write(table.concat(ss,",")..cc.."\n")
	
end

fp:write("};\n")

fp:write([[

int map_]]..name..[[(int x,int y)
{
	if( (x<0) || (x>=width) || (y<0) || (y>=height) )
	{ return -1; } // out of range
	int idx=bmap[x+y*width];
	return cmap[idx];
}

]])

fp:close()


local fp=io.open("art/"..name..".h","wb")
fp:write([[

extern int map_]]..name..[[(int x,int y);

]])
fp:close()


end

process("lenna")

