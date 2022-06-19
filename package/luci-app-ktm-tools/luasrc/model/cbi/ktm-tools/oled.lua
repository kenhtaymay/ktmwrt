m = Map("ktm-tools", "", "")

m:section(SimpleSection).template="ktm-tools/oled-status"

s = m:section(TypedSection, "oled", "")
s.anonymous=true

o = s:option(Flag, "enable", translate("Enable"))
o.default=0
o = s:option(Flag, "autoswitch", translate("Enable Auto switch"))
o.default=0
from = s:option(ListValue, "from", translate("From"))
to = s:option(ListValue, "to", translate("To"))
for i=0,23 do
	for j=0,30,30 do
		from:value(i*60+j,string.format("%02d:%02d",i,j))
		to:value(i*60+j,string.format("%02d:%02d",i,j))
	end
end
from:value(1440,"24:00")
to:value(1440,"24:00")
from:depends("autoswitch",'1')
to:depends("autoswitch",'1')
from.default=0
to.default=1440

require("nixio.fs")
local try_devices = nixio.fs.glob("/dev/i2c-[0-9]*")

o = s:option(ListValue, "i2cdevice", translate("I2C Device"),
	translate("Select I2C port."))
if try_devices then
	local node
	for node in try_devices do
		o:value(node, node)
	end
end

o = s:option(Value, "speedtesttime", translate("Speedtest interval(s)"), translate('Speedtest will run in set seconds'))
o.default=900

-- a = d:option(Value, "i2cdevice", "Name"); a.optional=false; a.rmempty = false;  -- name is the option in the cbi_file
return m