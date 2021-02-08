#change parameters[]
#add id

import os
import re
import json
import uuid
import shutil
import numpy as np
from PIL import Image
from cv2 import cv2
from pathlib import Path




main_path =    ['3d','3dplant','atlas','brush','surface']
main_pathfolder = ''
return_name = ''#所有每个文件夹名
exist_json = True
need_add_json = [] #需要添加json的文件夹名
type_folder = ''
id = ''
mesh = []
texture = {}
preview = []
thumbnail = []
map_json = []
lod_json = []
texelDens = []

#修改路径
def loop_mainfolder(main_pathfolder):
    for i in  ['3d','3dplant','atlas','brush','surface']:
        main_pathfolder = Path('F:/QuixelBridge_asset/Custom/'+i)
        type_folder == i
        find_all(main_pathfolder, return_name) #——>


class Params():
    
    def __init__(self, json_path):
        with open(json_path) as f:
            params = json.load(f)  # 将json格式数据转换为字典
            self.__dict__.update(params)

    def save(self, json_path):
        with open(json_path, 'w') as f:
            json.dump(self.__dict__, f, indent=1)  # indent缩进级别进行漂亮打印

    def update(self, json_path):
        """Loads parameters from json file"""
        with open(json_path) as f:
            params = json.load(f)
            self.__dict__.update(params)

    @property  # Python内置的@property装饰器就是负责把一个方法变成属性调用的
    def dict(self):
        """Gives dict-like access to Params instance by `params.dict['learning_rate']"""
        return self.__dict__


def change_json(main_pathfolder,return_name):
    parameters = {
        "origin": "bridge-custom",
        "name": "Omniglot",
        "categories": ["3d","branch"],
        "assetCategories":["3D asset","branch"],
        "tags": [],
        "environment": [""],
        "asset": 10,
        "id": 100,
        "version": 2,
        "previews": {"relativeSize":texture_relative_size},
        "searchStr": "AM_COG_City_Decos_Bench 9e466631-79b1-9042-a061-17d1b70cedc9 3d branch",
        "isMSAsset": False,
        "surfaceSize": "",
        "tilesVertical":False,
        "tilesHorizontal":False,
        "meta":[
            {"key":"scanArea","name":"Scan Area","value":"1x1 m"},
            {"key":"width","name":"Width","value":"0m"},
            {"key":"length","name":"Length","value":"0m"},
            {"key":"height","name":"Height","value":"0m"},
            {"key":"texelDensity","name":"Texel Density","value":texture_max},
            {"key":"assembly","name":"Assembly","value":"false"},
            {"key":"calibration","name":"Calibration","value":""},
            {"key":"color","name":"Color","value":"null"},
            {"key":"iteration","name":"Iteration Number","value":"%%iteration%%"},
            {"key":"set","name":"Set Toggle","value":"false"},
            {"key":"setname","name":"Set Name","value":"null"},
            {"key":"points","name":"Points","value":"0"},
            {"key":"scanner","name":"Scanner","value":""},
            {"key":"tileable","name":"Tileable","value":False}],
        "averageColor":None,
        "isExternal":True,
        "path":"F:\\QuixelBridge_asset\\Custom\\3d\\New folder_666",
        "maps":[
            {"name":" ","type":" ","uri":" ","resolution":"2048x2048","colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0}
            ],
        "preview":"F:\\QuixelBridge_asset\\Custom\\3d\\New folder_4\\a_preview_0.jpg",
        "lowRez":"F:\\QuixelBridge_asset\\Custom\\3d\\New folder_4\\a_preview_0.jpg",
        "lodList":[],
        "models":[],
        "thumbnail":"F:\\QuixelBridge_asset\\Custom\\3d\\New folder_4\\a_thumbnail.jpg"}    

    json_str = json.dumps(parameters, indent=4)
    with open(str(main_pathfolder)+'\\'+str(return_name)+'\\'+'assetData.json', 'w') as f:  # 创建一个params.json文件
        f.write(json_str)  # 将json_str写到文件中
    id = str(uuid.uuid1()) 
    
    params = Params(str(main_pathfolder)+'\\'+str(return_name)+'\\'+'assetData.json')
    params.name = str(return_name)   # 修改json中的数据
    params.id = id 
    params.path =str(main_pathfolder),return_name
    params.searchStr = return_name,id,str(main_pathfolder)
    params.lodList= lod_json #fbx的名字
    params.models =  lod_json
    params.maps = map_json
    params.preview = preview
    params.lowRez = preview
    params.thumbnail = thumbnail
    params.save(str(main_pathfolder)+'\\'+str(return_name)+'\\'+'assetData.json')  # 将修改后的数据保存



#get all file name
def find_all(main_pathfolder, return_name):
    dir = main_pathfolder
    for filename in os.listdir(dir):
        #print(filename)
        return_name = filename
        if exist_json == True:
            search_json(main_pathfolder,return_name,need_add_json)#——>
    return return_name


def search_json(main_pathfolder,return_name,need_add_json):
    json_namepath= str(main_pathfolder)+'\\'+return_name+'\\'+'assetData.json'
    #need_add_json = []
    if os.path.isfile(json_namepath):
        #print(str(return_name),'have json')
        pass
    else:
        #文件夹中没有json的才会显示
        need_add_json.append(return_name)#添加到数组中
        #print(str(return_name),'need add json')
        
        get_filename(main_pathfolder,return_name,mesh,texture,preview,thumbnail,map_json,lod_json) #——>
        change_json(main_pathfolder,return_name)#——>
        return need_add_json

def get_filename(main_pathfolder,return_name,mesh,texture,preview,thumbnail,map_json,lod_json):
    ''' 获取指定目录下的所有指定后缀的文件名 '''
    #str(main_pathfolder)+'\\'+return_name
    path = str(main_pathfolder)+'\\'+return_name #指到每个文件夹的名字的路径
    f_list = os.listdir(path) # 文件夹下的所有文件名
    texture_width = []
    texture_length = []



    for i  in f_list:#mesh
        stuffix = os.path.splitext(i)[1].lower()  # os.path.splitext():分离文件名与扩展名
        if stuffix == '.fbx' or stuffix == '.obj':
            mesh.append(i) 
    for i in mesh: 
        prefix = os.path.splitext(i)[0].lower()
        lod_json.append({"name":str(prefix),"type":"lod","uri":str(i),"resolution":"2048x2048","bitDepth":8,"contentLength":0})
    
    for x in f_list: #texture
        stuffix = os.path.splitext(x)[1].lower()
        prefix = os.path.splitext(x)[0].lower()
        if (stuffix == '.tga' or stuffix == 'exr'  or stuffix == 'tiff' ) and ('albedo' in prefix or 'displacement' in prefix or '_normal_' in prefix or  '_bump_' in prefix or 'gloss' in prefix or 'roughness' in prefix or 'cavity' in prefix or 'specular' in prefix or '_normalbump_' in prefix or '_mask_' in prefix or '_ao' in prefix or '_metalness'in prefix or '_cus_'in prefix or 'translucency' in prefix or'opacity' in prefix or 'transmission' in prefix ):#确认贴图名称是否准确        
            #texture[x] = '2048x2048'
            a=Image.open(str(path)+'\\'+x)
            a.save(str(path)+'\\'+ str(prefix) +'.png')
            #TODO image size
            #shutil.move((str(path)+'\\'+x),(str(path)+'\\'+'otherfiles'+'\\'+x))
        if (stuffix == '.jpg' or stuffix == '.png') and ('albedo' in prefix or 'displacement' in prefix or '_normal_' in prefix or  '_bump_' in prefix or 'gloss' in prefix or 'roughness' in prefix or 'cavity' in prefix or 'specular' in prefix or '_normalbump_' in prefix or '_mask_' in prefix or '_ao' in prefix or '_metalness'in prefix or '_cus_'in prefix or 'translucency' in prefix or'opacity' in prefix or 'transmission' in prefix):#确认贴图名称是否准确
            #texture.append(x)
            texture_path = str(path)+'\\'+x #路径加贴图名
            try:
                img = cv2.imread(texture_path)  
                size = str(img.shape[0])+'x'+str(img.shape[1]) #完整格式
                texture[x] = size
                texture_width.append(img.shape[0])
                texture_length.append(img.shape[1])
                texelDens.append((img.shape[0] + img.shape[1])/(2.0*8192.0))  #每张图的相对大小是8192px/m
                

                #todo 查询贴图信息
                #print('texture.keys',texture.keys())
            except:
                print('error:',x)

        if (stuffix == '.jpg' or stuffix == '.png' or stuffix == '.tga') and ('preview' in prefix ):
            preview.append(x)
        if (stuffix == '.jpg' or stuffix == '.png' or stuffix == '.tga') and ('thumbnail' in prefix ):
            thumbnail.append(x)  

    #计算贴图的最大值       
    texture_max_w = max(texture_width)
    texture_max_l = max(texture_length)
    global texture_max
    texture_max = str(max(texture_max_w,texture_max_l))+'px/m'
    global texture_relative_size
    str_texture_w = int(texture_max_w/8192)
    str_texture_l = int(texture_max_l/8192)
    texture_relative_size = str(str_texture_w)+"x"+str(str_texture_l)
    #print(texture_relative_size)
    #w = (texture_max_w/8192)/(texture_max_l/8192)
    #print ("w:",(texture_max_w/8192),"x",(texture_max_l/8192))
    add_maps(texture)
    


def add_maps(texture):
    

    for key, value in texture.items():#贴图数据添加到json上
        print( key, value)
        
        prefix = os.path.splitext(key)[0].lower()
        #stuffix = os.path.splitext(key)[1].lower()



        if 'albedo' in prefix:
            map_json.append({"name":'Albedo',"type":"albedo","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if 'displacement' in prefix:
            map_json.append({"name":'Displacement',"type":"displacement","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})            
        if '_normal_' in prefix:
            map_json.append({"name":'Normal',"type":"normal","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if '_bump_' in prefix:
            map_json.append({"name":'Bump',"type":"bump","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if 'gloss' in prefix:
            map_json.append({"name":'Gloss',"type":"gloss","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if 'roughness' in prefix:
            map_json.append({"name":'Roughness',"type":"roughness","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})                        
        if 'cavity' in prefix:
            map_json.append({"name":'Cavity',"type":"cavity","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if '_normalbump_' in prefix:
            map_json.append({"name":'Normalbump',"type":"normalbump","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if '_mask_' in prefix:
            map_json.append({"name":'Mask',"type":"mask","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if '_normalbump_' in prefix:
            map_json.append({"name":'Normalbump',"type":"normalbump","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if '_ao' in prefix:
            map_json.append({"name":'AO',"type":"ao","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})     
        if 'metalness' in prefix:        
            map_json.append({"name":'Metalness',"type":"metalness","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})   
        if '_cus_' in prefix:
            map_json.append({"name":'Custom',"type":"custom","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})     
        if 'translucency' in prefix:
            map_json.append({"name":'Translucency',"type":"translucency","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})      
        if 'opacity' in prefix:
            map_json.append({"name":'Opacity',"type":"opacity","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if 'specular' in prefix:
            map_json.append({"name":'Specular',"type":"specular","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
        if 'transmission' in prefix:
            map_json.append({"name":'Transmission',"type":"transmission","uri":key,"resolution":value,"colorSpace":"","averageColor":"","mimeType":"","bitDepth":8,"contentLength":0})
                         
    #return mesh,texture,preview,thumbnail, map_json,lod_json


loop_mainfolder(main_pathfolder)

