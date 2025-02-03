import bpy
import os

folder_path = "ADD PATH HERE" #FIXME
file_prefix = "ADD PREFIX HERE" #FIXME ("all"; "p"; "groupX_")

for ob in bpy.data.objects:
	ob.hide_set(False)
	ob.hide_viewport = False
	ob.hide_select = False
bpy.ops.object.select_all(action="SELECT")
bpy.ops.object.delete(use_global=False)

if not os.path.exists(folder_path):
    exit()
obj_files = [f for f in os.listdir(folder_path) if f.endswith(".obj") and f.startswith(file_prefix)]

for obj_file in obj_files:
	bpy.ops.wm.obj_import(filepath=os.path.join(folder_path, obj_file), up_axis="Z", forward_axis="Y")

bpy.data.scenes["Scene"].frame_end = 0

min1, min2 = 10000, 10000
for ob in bpy.data.objects:
	i = float(ob.name[len(file_prefix):])
	if min1 > i: min2 = min1; min1 = i
	elif min2 > i: min2 = i

delta = min2 - min1
for ob in bpy.data.objects:
	i = int(float(ob.name[len(file_prefix):]) / delta)
	ob.hide_viewport = True
	ob.keyframe_insert(data_path="hide_viewport", frame=0)
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3-1)
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3+3)
	ob.hide_viewport = False
	ob.keyframe_insert(data_path="hide_viewport", frame=-1)
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3)
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3+2)
	if bpy.data.scenes["Scene"].frame_end < i*3+2: bpy.data.scenes["Scene"].frame_end = int(i*3+2)
