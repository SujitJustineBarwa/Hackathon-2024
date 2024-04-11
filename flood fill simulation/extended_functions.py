class state_register:
    def __init__(self):
        self.curr_state = {}
        self.prev_state = {}
        
    def add(self,name,value):
        self.curr_state[name] = value
        self.prev_state[name] = value
        
    def update_prev(self,name,value):
        self.prev_state[name] = value
        
    def update_curr(self,name,value):
        self.curr_state[name] = value
        
    def show(self):
        print(self.curr_state)
        print(self.prev_state)
        print()