import bpy
import os

for ob in bpy.data.objects:
	ob.hide_set(False)
	ob.hide_viewport = False
	ob.hide_select = False
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)

folder_path = "ADD PATH HERE" #FIXME
if not os.path.exists(folder_path):
    print("No folder")
    exit()
obj_files = [f for f in os.listdir(folder_path) if f.endswith('.obj') and f.startswith('all')]

for obj_file in obj_files:
	print(f"Importation de {obj_file}...")
	bpy.ops.wm.obj_import(filepath=os.path.join(folder_path, obj_file), up_axis="Z")

bpy.types.Scene.frame_end = 0

for ob in bpy.data.objects:
	i = float(ob.name[3:])
	ob.hide_viewport = True
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3-1)
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3+3)
	ob.hide_viewport = False
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3)
	ob.keyframe_insert(data_path="hide_viewport", frame=i*3+2)
	if bpy.types.Scene.frame_end < i*3+2: bpy.types.Scene.frame_end = i*3+2
