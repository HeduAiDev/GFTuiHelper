
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
    
    c = tui.create_form(data,
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
    c = c | {"align_h": "center"} | {"vframe": True} 
    b = tui.create_button("b", lambda :1)
    d = tui.create_button("d", lambda :1)
    e = tui.create_button("e", lambda :1)
    f = tui.create_button("f", lambda :1)
    l = layout('split', [[c, '---', f], '|', [b, '---', d], '|', e ])
    tui.start_menu_loop(l, type="full_screen")

    