module("luci.controller.ktm-tools", package.seeall)
function index()
    if not nixio.fs.access("/etc/config/ktm-tools") then
		return
	end
    local page = entry({"admin", "services", "ktm-tools"}, firstchild(), "KTM Tools", 60)

	page.dependent = true
	page.acl_depends = { "luci-app-ktm-tools" }

    entry({"admin", "services", "ktm-tools", "oled"}, cbi("ktm-tools/oled"), "Oled Config", 1)
    entry({"admin", "services", "ktm-tools", "ttl"}, cbi("ktm-tools/ttl"), "TTL Config", 2)
    entry({"admin", "services", "ktm-tools", "about"}, cbi("ktm-tools/about"), "About", 3)
    entry({"admin", "services", "ktm-tools", "oled-status"}, call("act_status"))
end

function act_status()
	local e={}
	e.running = luci.sys.call("pgrep -f /usr/bin/oled > /dev/null")==0
	luci.http.prepare_content("application/json")
	luci.http.write_json(e)
end 