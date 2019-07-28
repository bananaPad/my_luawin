down=false
x0=0
y0=0
x=0
y=0
lines={}
function ondraw( cnt )
	for i,v in ipairs(lines) do
		cnt:setrgb(1,0,0)
		cnt:moveto(v.x0,v.y0)
		cnt:lineto(v.x1,v.y1)
	end
	cnt:moveto(x0,y0)
	cnt:lineto(x,y)
	cnt:stroke()
end
function onkeydown( key )
	print(key)
end
function onexit( )
	print("exit")
end
function onlbup(m )
	down=false
	table.insert(lines,{x0=x0,y0=y0,x1=x,y1=y})
end
function onlbdown( m)
	down=true
	x0=m.x
	y0=m.y
end
function onmove( m )
	if down then
		x=m.x
		y=m.y
		window.redraw()
	end
end
window.settitle("first")
window.setposition(100,100)
window.setwidthheight(200,200)
window.setexit(onexit)
window.setlbdown(onlbdown)
window.setlbup(onlbup)
window.setmousemove(onmove)
window.setredraw(ondraw)
window.setkeydown(onkeydown)
