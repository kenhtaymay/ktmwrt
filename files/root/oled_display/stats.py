import threading
import queue
import speedtest
import socket

import time
import SSD1306

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import subprocess

disp = SSD1306.SSD1306_128_64()
disp.begin()
disp.clear()
disp.display()

width = disp.width
height = disp.height
image = Image.new('1', (width, height))

draw = ImageDraw.Draw(image)
draw.rectangle((0,0,width,height), outline=0, fill=0)

padding = -2
top = padding
bottom = height-padding

x_download = 90

font8 = ImageFont.truetype('ktm.ttf', 8)
font12 = ImageFont.truetype('ktm.ttf', 12)
font16 = ImageFont.truetype('ktm.ttf', 16)
font32 = ImageFont.truetype('ktm.ttf', 32)


oledQueue = queue.Queue()
oledQueue.put(lambda: (draw.rectangle((0,0,width,height), outline=0, fill=0),draw.text((0, 0), "--"+(b'\x00').decode("utf-8"),  font=font32, fill=255),draw.text((90, 0), "--",  font=font16, fill=255),draw.text((90, 20), "--",  font=font16, fill=255)))

# Define Error Logging
def printerror(ex):
	print('\033[31m' + str(ex) + '\033[0m')

def oledQueueDisplay():
    while True:
        if oledQueue.empty() == False:
            f = oledQueue.get()
            f()
            disp.image(image)
            disp.display()
        else:
            time.sleep(0.005)

def getTemp():
    cmd = "cat /sys/class/thermal/thermal_zone0/temp | tr -d $'\n'"
    while True:
        try:
            temp = int(subprocess.check_output(cmd, shell = True ).decode('utf-8'))
            for x in range(10):
                read_temp = int(subprocess.check_output(cmd, shell = True ).decode('utf-8'))
                temp = (temp+read_temp)/2
                time.sleep(.5)
            oledQueue.put(lambda: (draw.text((0, 0), (b'\x03\x03\x03').decode("utf-8"),  font=font32, fill=0),draw.text((0, 0), str(int(temp/1000))+(b'\x00').decode('utf-8'),  font=font32, fill=255)))
        except Exception as e:
            printerror("An unknown error occured while get temperature.")
            printerror(e)
        
        time.sleep(2)

done_download = True
def blinkDownload():
    while done_download == False:
        oledQueue.put(lambda: draw.text((77, 0), (b'\x03').decode("utf-8"),  font=font16, fill=0))
        time.sleep(0.5)
        oledQueue.put(lambda: draw.text((77, 0), (b'\x01').decode('utf-8'),  font=font16, fill=255))
        time.sleep(0.5)

done_upload = True
def blinkUpload():
    while done_upload == False:
        oledQueue.put(lambda: draw.text((77, 20), (b'\x03').decode("utf-8"),  font=font16, fill=0))
        time.sleep(0.5)
        oledQueue.put(lambda: draw.text((77, 20), (b'\x02').decode('utf-8'),  font=font16, fill=255))
        time.sleep(0.5)	

#Check or internet connection
def is_connected():
    try:
        # connect to the host -- tells us if the host is actually
        # reachable
        socket.create_connection(("www.google.com", 80))
        return True
    except OSError:
        pass
    return False


def getSpeedTest():
    global done_download, done_upload
    while True:
        try:
            result = is_connected()
            if is_connected() == False:
                printerror("No network connection was found!")
                # lcdQueue.put(lambda: display.lcd_display_string("NoNetwork ", 1))
                time.sleep(30)
                continue

            #Download speed
            done_download = False
            tdown = threading.Thread(target=blinkDownload)
            tdown.start()
            st = speedtest.Speedtest()
            download = st.download()
            download = round(download / 1000 / 1000, 2)
            done_download = True
            
            # Print to display
            tdown.join()
            download_s = str(int(round(download, 0)))
            # print(download_s)
            oledQueue.put(lambda: (draw.text((90, 0), (b'\x03\x03\x03').decode("utf-8"),  font=font16, fill=0),draw.text((90, 0), download_s,  font=font16, fill=255)))


            # Upload speed
            done_upload = False
            tup = threading.Thread(target=blinkUpload)
            tup.start()
            upload = st.upload()
            upload = round(upload / 1000 / 1000, 2)
            done_upload = True

            # Print to display
            tup.join()
            upload_s = str(int(round(upload, 0)))
            # print(upload_s)
            oledQueue.put(lambda: (draw.text((90, 20), (b'\x03\x03\x03').decode("utf-8"),  font=font16, fill=0),draw.text((90, 20), upload_s,  font=font16, fill=255)))
            time.sleep(30)
        except Exception as e:
            done_download = True
            done_upload = True
            printerror("An unknown error occured while get speedtest.")

def getMem():
    cmd = "free -m | awk 'NR==2{printf \"%.2f\", $3*100/$2 }'"
    ip_cmd = "ifconfig br-lan | awk -F: 'NR==2{print $2}' | awk '{print $1}'"
    while True:
        try:
            MemUsage = float(subprocess.check_output(cmd, shell = True ))
            MyIP = subprocess.check_output(ip_cmd, shell=True).decode("utf-8")
            oledQueue.put(lambda: (draw.text((0, 40), MyIP,  font=font12, fill=255),draw.rectangle((0, 58, 127, 63), outline=255, fill=0), draw.rectangle((3, 60, int(121/100*MemUsage), 61), outline=255, fill=0)))
        except Exception as e:
            printerror("An unknown error occured while get memory.")
            printerror(e)
        
        time.sleep(2)

def pingDisp():
    while True:
        oledQueue.put(lambda: draw.text((110, 42), (b'\x03\x03').decode("utf-8"),  font=font12, fill=0))
        time.sleep(1)
        oledQueue.put(lambda: draw.text((110, 42), "TM",  font=font12, fill=255))
        time.sleep(1)	

#Main
if __name__ == '__main__':
    threading.Thread(target=oledQueueDisplay).start()
    threading.Thread(target=getTemp).start()
    threading.Thread(target=getSpeedTest).start()
    threading.Thread(target=getMem).start()
    threading.Thread(target=pingDisp).start()