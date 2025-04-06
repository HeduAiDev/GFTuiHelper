from ._C import *
from enum import Enum
from collections import deque
class SplitLayoutType(Enum):
    Block4 = 1
    Block3Up = 2
    Block3Down = 3
    Block3Left = 4
    Block3Right = 5
    Block2Horizontal = 6
    Block2Vertical = 7
    
def layout(type, config: list) -> ComponentBase:
    # 1. a | b        [[a, '|', b],
    #    -----   =>    '---------',
    #    c | d          [c, '|', d]]
    #
    # 2. a           [a,
    #   ------  =>    '---------',
    #   b | c         [b, '|', c]]
    #
    # 3. a | b       [[a, '|', b],
    #    -----  =>    '---------',
    #      c          c]
    #
    # 4. a | b
    #      |---  =>  [a, '|', [b, '---', c]]
    #      | c       
    # 
    # 5. a | b
    #    --|     =>  [[a, '---', c], '|', b]
    #    c |
    #
    # 6. a | b  =>   [a, '|', b]
    #
    # 7. a 
    #   ---     =>   [a, '---', b]
    #    b

    if type == "split":
        is_hseparator = lambda x: isinstance(x, str) and  x.startswith('---')
        is_vseparator = lambda x: isinstance(x, str) and  x.startswith('|')
        is_component = lambda x: isinstance(x, ComponentBase)
        is_list = lambda x: isinstance(x, (list, tuple))
        is_item = lambda x: is_component(x) or is_list(x)
        is_split_h = lambda x: is_list(x) and len(x) == 3 and all([f(c) for c, f in zip(x, [is_component, is_hseparator, is_component])])
        is_split_v = lambda x: is_list(x) and len(x) == 3 and all([f(c) for c, f in zip(x, [is_component, is_vseparator, is_component])])
        cc = collect_components = lambda x: [c for c in x if is_component(c)]
        dq = deque(config)
        while len(dq) > 1:
            if len(dq) < 3 or (not is_hseparator(dq[1]) and not is_vseparator(dq[1])):
                raise ValueError("Invalid layout configuration")
            c = [dq.popleft() for _ in range(3)]
            # block4
            if is_split_v(c[0]) and is_hseparator(c[1]) and is_split_v(c[2]): 
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block4, cc(c[0] + c[2])))
            elif is_split_h(c[0]) and is_vseparator(c[1]) and is_split_h(c[2]): 
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block4, [c[0][0], c[2][0], c[0][2], c[2][2]]))
            # block3up
            elif is_component(c[0]) and is_hseparator(c[1]) and is_split_v(c[2]):
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block3Up, [c[0], *cc(c[2])]))
            # block3down
            elif is_split_v(c[0]) and is_hseparator(c[1]) and is_component(c[2]):
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block3Down, [*cc(c[0]), c[2]]))
            # block3left
            elif is_component(c[0]) and is_vseparator(c[1]) and is_split_h(c[2]):
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block3Left, [c[0], *cc(c[2])]))
            # block3right
            elif is_split_h(c[0]) and is_vseparator(c[1]) and is_component(c[2]):
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block3Right, [*cc(c[0]), c[2]]))
            # block2horizontal
            elif is_component(c[0]) and is_vseparator(c[1]) and is_component(c[2]):
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block2Horizontal, [c[0], c[2]]))
            # block2vertical
            elif is_component(c[0]) and is_hseparator(c[1]) and is_component(c[2]):
                dq.appendleft(layout_resizable_split(SplitLayoutType.Block2Vertical, [c[0], c[2]]))
            else:
                dq.extendleft([ci if not is_list(ci) else layout(type, ci) for ci in c])
        return dq[0]
    else:
        raise ValueError(f"Invalid layout type {type}")
        
    
