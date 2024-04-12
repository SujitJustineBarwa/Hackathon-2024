class memory:
    def __init__(self):
        self.status = {}
        
    def add(self,name,type):
        if name not in self.status.keys():
            self.status[name] = type
        
    def update(self,name,info):
        self.status[name] = info
        
    def reset(self):
        for key,value in self.status.items():
            self[key] = False if type(value) is bool else [] 