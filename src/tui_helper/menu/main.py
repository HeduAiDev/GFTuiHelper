
import tui_helper as tui
from time import sleep

def main():
    data = tui.RefData()
    def on_change(label, content):
        if label == "GF_TASK_TYPE: ":
            cnt = data["GF_TASK_TYPE: "]
            if cnt == "0":
                data["GF_ENV2: "]= "mindie_train"
                return "mindie_infer"
        
        return content
    
    c = tui.create_form_with_data(data,
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
    c = c | {"align_h": "right"}
    tui.start_menu_loop(c)
    