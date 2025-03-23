
import tui_helper as tui

def main():
    def on_change(label, content):
        # if label == "GF_ENV2: " and len(content) > 0:
        #     new_c = content[-1]
        #     if not new_c.isdigit():
        #        content = content[:-1]
        # if content == 'mindie_infer':
        #     return 'train'
        return content


    tui.start_menu_loop(
    """
    [
        [{"input_type": "number", "label": "GF_ENV: ", "placeholder": "right click to paste", "input_style":{"align_h": "left", "max_width": 10}, "label_style":{"align_h": "center"}},
        {"input_type": "text", "label": "GF_ENV2: ", "placeholder": "right click to paste"}
        ],
        {"input_type": "text", "label": "GF_MODEL: ", "placeholder": "right click to paste"},
        {"input_type": "select", "label": "GF_TASK_TYPE: ", "entries": ["train", "mindie_infer", "vllm_infer"], "default_index": 2}
    ]
    """,
    on_change
    )