m = Map("ktm-tools", "", "")

m:section(SimpleSection).template="ktm-tools/about"

-- a = d:option(Value, "i2cdevice", "Name"); a.optional=false; a.rmempty = false;  -- name is the option in the cbi_file
return m