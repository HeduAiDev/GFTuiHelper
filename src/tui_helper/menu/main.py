
import tui_helper as tui
from tui_helper import layout
def main():
    data = tui.RefData()
    def on_change(label, content):
        if label == "GF_MODEL: ":
            data["GF_ENV2: "] = data["GF_MODEL: "]
        if label == "GF_ENV2: ":
            data["GF_MODEL: "] = data["GF_ENV2: "]
        if label == "GF_TASK_TYPE: ":
            data["GF_ENV2: "]= content
        
        return content
    
    c = tui.form(data,
    [
        [
            {"input_type": "number", "label": "GF_ENV: ", "placeholder": "right click to paste", "input_style":{"align_h": "left", "max_width": 10}, "label_style":{"align_h": "center"}},
            {"input_type": "text", "label": "GF_ENV2: ", "placeholder": "right click to paste"}
        ],
        {"input_type": "text", "label": "GF_MODEL: ", "placeholder": "right click to paste"},
        {"input_type": "select", "label": "GF_TASK_TYPE: ", "entries": ["train", "mindie_infer", "vllm_infer"], "default_index": 2}
    ],
    on_change
    )
    
    a = tui.button("a")
    b = tui.button("b")
    c = c | {"align_h": "center"} | {"vframe": True} 
    d = tui.button("d")
    e = tui.button("e")
    f = tui.button("f")
    g = tui.button("g")
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
    # l = layout('split', [[a, '|', b],
    #                     '---------',
    #                     [c, '|', d]])
    # l = layout('split', [[[a, "|", b, "|", c, '----', [d, '|', e, '|', f]], '-----', a], '|', [b, '----', c]])
    # l = layout('split', [[[a, '---', b, '---', d], '---', e], '|', [f,'---', g ]])
    l = layout('split', [[[a, "|", b], "---", [c, "|", d]], "|" , [[e, "|", f], "---", [g, "|", a]]])
    tui.start_menu_loop(l, type="fit_component")