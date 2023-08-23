use=[
    'start','爱理','安洁',
    '妹','咪呜',
    '前三选项'
]
import json
with open('namemap.json','r',encoding='utf8') as ff:
    mp=json.load(ff)
savemap={}
mpmp={}
for _u in use:
    with open(f'ja/{_u}.txt','r',encoding='utf8') as ff:
        ja=ff.read()
    with open(f'zh/{_u}.txt','r',encoding='utf8') as ff:
        zh=ff.read()
        
    zh=zh.split('=========')
    ja=ja.split('=========')
    for  i in range(min(len(zh),len(ja))):
        jai=ja[i].replace('\n','').replace(' ','')
        zhi=zh[i].replace('\n','').replace(' ','')
        if(len(jai)==0):continue
        if(jai not in mp):
            if jai in savemap and savemap[jai]!=zhi:
                print(jai,zhi,savemap[jai])
            savemap[jai]=zhi
            
            if '襞' in jai:
                savemap[jai.replace('襞','ひだ')]=zhi
remap={
         
        'なんだかんだで、ついつい考えてしまう。\\nあとほんの少しで着くというのに、それこそ男の性のように。':'なんだかんだで、ついつい考えてしまう。\\nあとほんの少しで着くというのに、それこそ男の性ように。',
        '空気なんて、きっと桃色で―':'空気なんて、きっと桃色で――',
         
          "隼太と桜乃もようやく調子を取り戻したのか\\nそれぞれの感想を口にする。":"隼太と桜乃もようやく調子を取り戻したか、\\nそれぞれの感想を口にする。", 
        #  "根元からべっとりと桜乃のものに濡れ、\\nからみつくひだが裏返りそうなほどに貼りついてくる。":   "根元からべっとりと桜乃のものに濡れ、\\nからみつく襞が裏返りそうなほどに貼りついてくる。"
    }
for k in remap:
    savemap[k]=savemap[remap[k]] 


with open('translate.json','w',encoding='utf8') as ff:
    ff.write(json.dumps(savemap,ensure_ascii=False,indent=3))

