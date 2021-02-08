

import os,time
import sys
import json
import pandas as pd
#import xlsxwriter
from pathlib import Path
#import PrintFileAsJson_07 as PFAJ


#workbook = xlsxwriter.Workbook('F:\\QuixelBridge_asset\\Custom\\QB_assets1.xlsx')
#worksheet = workbook.add_worksheet()
json_list = {}
custom = 'custom'
download = 'Downloaded'



def findJson(subasset,json_list):
    for i in  ['3d','3dplant','atlas','brush','surface']:
        main_pathfolder = Path('F:/QuixelBridge_asset/'+subasset+'/'+i)
        print(main_pathfolder)
        for filename in os.listdir(main_pathfolder):
            #print(filename)
            namepath = str(main_pathfolder)+'/'+filename
            file_change = time.ctime(os.stat(namepath).st_mtime)
            file_create = time.ctime(os.stat(namepath).st_ctime)
            #json_namepath= str(main_pathfolder)+'\\'+filename+'\\'+'assetData.json'
            #namepath = str(main_pathfolder)+'\\'+filename
            #if os.path.isfile(json_namepath):
                #print(json_namepath)#文件夹路径
            for x in os.listdir(namepath):

                if os.path.splitext(x)[1]  == '.json':
                    json_namepath = str(namepath+'/'+x)
                    with open(json_namepath, 'r') as f:

                        data = json.load(f)
                        meta = data["meta"]
                        maps_x = []
                        if "maps" in data:
                            maps = data["maps"]
                            for i in maps:
                                maps_x.append(i.get('name'))
                        maps_names = sorted(set(maps_x),key=maps_x.index)
                                #print(maps_name)

                        try:
                            height = [x for x in meta if x.get("key")=="height"][0].get("value")
                            texel_density = [x for x in meta if x.get("key")=="texelDensity"][0].get("value")
                            scan_area = [x for x in meta if x.get("key")=="scanArea"][0].get("value")

                        except:
                            height = "0m"
                            texel_density = "0 px/m"
                            scan_area = "0x0 m"
                            maps_name = " "

                        #print("height",height)
                        #print(data['name'])#上传到QB显示的名字
                        #print(filename)
                        my_temp_dict = {filename:{"Name":data['name'],"Texel Density":texel_density,"Height":height,"Scan Area":scan_area ,"Relative Size":data['previews']['relativeSize'],"File Create":file_create,"File Change":file_change, "Maps Name":maps_names,"Path":namepath}}
                        #insert_image(str(data['previews']))
                        json_list.update(my_temp_dict)

                else:
                    pass


def createExcel(json_list):
    #print (json_list)
    df = pd.DataFrame(dict(json_list)).T
    df.to_excel('F:\\QuixelBridge_asset\\Custom\\QB_assets.xlsx', encoding='utf-8', index=False)

findJson(custom,json_list)
findJson(download,json_list)

createExcel(json_list)


