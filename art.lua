

local wgrd = require "wetgenes.grd"


local function process(name,cx,cy)

print( "Processing art/"..name..".png into a c file")

local g=wgrd.create( "art/"..name..".png" )
g:convert(wgrd.FMT_U8_INDEXED) -- make sure it is indexed
--g:cmap() -- sort cmap
local clip={x=0,y=0,z=0,w=g.width,h=g.height,d=1}
g:shrink( clip )

local fp=io.open("art/"..name..".c","wb")

local cmap=g:palette(0,256)
local bmap=g:pixels(clip.x,clip.y,0,clip.w,clip.h,1)

fp:write("// This file is machine generated and should not be edited...\n")
fp:write("\n")

fp:write(string.format("static const int hx=%d;\n",clip.w))
fp:write(string.format("static const int hy=%d;\n",clip.h))
fp:write(string.format("static const int cx=%d;\n",cx-clip.x))
fp:write(string.format("static const int cy=%d;\n",cy-clip.y))
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
for y=1,clip.h do

	local cc=","
	if y==clip.h then cc="" end

	local ss={}

	for x=1,clip.w do

		ss[#ss+1]=string.format("0x%02X",bmap[idx])
		idx=idx+1
	end
	
	fp:write(table.concat(ss,",")..cc.."\n")
	
end

fp:write("};\n")

fp:write([[

int map_]]..name..[[(int x,int y)
{
	x+=cx;
	y+=cy;
	if( (x<0) || (x>=hx) || (y<0) || (y>=hy) )
	{ return -1; } // out of range
	int idx=bmap[x+y*hx];
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

process("face",120,120)
process("hours",120,120)
process("minutes",120,120)
process("seconds",120,120)

