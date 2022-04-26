import os

import numpy as np
import pandas as pd
import plotly.express as px

STAT_PATH = "stats/"
PLOT_FOLDER = "plots/"


TEST_CASES = ("abundant", "random", "sparse", "multilevel")
TYPE_NAME = {
    1: "sfork_file",
    2: "sfork_populate",
    3: "sfork_lazy",
    4: "shared_mem"
}
COL_MAP = {
    "user_time" : 0,
    "system_time": 1,
    "wall_time(ms)": 2,
    "size(MB)": 3,
    "ret_value": 4
}
TYPES = TYPE_NAME.keys()
SIZES = [32, 64, 128, 256, 512, 1024]

def plot_test(test: str) -> None:

    data = []
    for type in TYPES:
        raw_data = {k: [] for k in SIZES}
        data_path = os.path.join(STAT_PATH, f"{test}_{type}.txt")
        if os.path.exists(data_path):
            arr = np.loadtxt(data_path, np.float64)

            for row in arr:
                row[0:3] = 1000 * row[0:3]
                raw_data[row[COL_MAP["size(MB)"]]].append(row)
        else:
            print(f"No data file: {data_path}")
        
        for size, d in raw_data.items():
            data.append(list(np.mean(np.array(d), axis=0)) + [TYPE_NAME[type]])
    
    df = pd.DataFrame(data, columns=list(COL_MAP.keys()) + ["Type"])
    # print(df.head())

    fig = px.line(
        df,
        x="size(MB)", 
        y="wall_time(ms)", 
        color="Type",
        title=f"Testcase: {test}",
	    # log_y = True,
	    log_x = True,
        markers=True)
    
    fig.update_layout(
       xaxis = dict(
            tickmode = 'array',
            tickvals = SIZES,
        )
    )
    # fig.update_traces(textposition="bottom left")

    fig.write_image(f"{PLOT_FOLDER}/plot_{test}.png")


def main() -> None:

    for test in TEST_CASES:
        plot_test(test)

if __name__ == "__main__":
    main()


