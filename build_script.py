Import("env")
import os

# print(env.Dump())

print ("###########################################################")
env.Replace(PROGNAME="%s" % env['PIOENV'] + "_sV" + env.GetProjectOption("custom_prog_version"))
print (env['PROGNAME'])

def copy_firmware(source, target, env):
	print("TEST MOVE FIRMWARE")
	# print(env.Dump())
	# print(source[0].get_abspath())
	# print(target[0].get_abspath())
	# src = env['PROJECT_BUILD_DIR'] + "/" + env['PIOENV'] + "/" + env['PROGNAME'] + ".bin"
	src = env['PROJECT_BUILD_DIR'] + "/" + env['PIOENV'] + "/firmware.bin"
	trg = "/tmp/" + env['PROGNAME'] + ".bin"
	print (src)
	print (trg)
	print ("END____________________________")
	os.rename(src,trg)

env.AddPostAction("buildprog", copy_firmware)

# print (env.Dump())