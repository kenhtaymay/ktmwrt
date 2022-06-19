m = Map("ktm-tools", "", "")

s = m:section(TypedSection, "ttl", "")
s.anonymous=true

o = s:option(Flag, "enable", translate("Enable"))
o.default=0

o = s:option(Value, "ttlset", translate("TTL"))
o.default=65

o = s:option(ListValue, "interface", translate("Interface"))
for k, v in pairs(luci.sys.net.devices()) do
	o:value(v, v)
end

return m