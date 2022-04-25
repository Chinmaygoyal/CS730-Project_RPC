import os

import numpy as np
import pandas as pd
import plotly.express as px

STAT_PATH = "stats/"
PLOT_FOLDER = "plots/"


TEST_CASES = ("abundant", "random", "sparse")
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
    print(df.head())

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
    data = []
    data_ws = []
    return 

    for N in Ns:
        for P in Ps:
            file_path = os.path.join(STAT_PATH, f"mpi_{P}_{N}.txt")
            if os.path.exists(file_path):
                arr = np.fromfile(file_path, np.float64, count=5, sep='\n')
                row1 = [N, P, 1/np.mean(arr), np.std(arr)]
                row2 = [N, P * ((Ns[0]/N) ** 3), 1/np.mean(arr), np.std(arr)]
                data.append(row1)
                if N//P == 8:
                    data_ws.append(row2)

    
    if not os.path.exists(PLOT_FOLDER):
        os.mkdir(PLOT_FOLDER)
    
    X_LABEL = f"P*({Ns[0]}/N)^3"

    df = pd.DataFrame(data, columns=["N", "P", "1/time_mean", "time_std"])

    df_ws = pd.DataFrame(data_ws, columns=["N", X_LABEL, "1/time_mean", "time_std"])

    fig = px.line(
        df_ws,
        x=X_LABEL, 
        y="1/time_mean", 
        text="N", 
        title="Weak Scaling",
	    log_y = True,
	    log_x = True,
        markers=True)
    
    fig.update_layout(
       xaxis = dict(
            tickmode = 'array',
            tickvals = Ps[1:],
        )
    )
    fig.update_traces(textposition="bottom left")

    fig.write_image(f"{PLOT_FOLDER}/plot_ws.svg")
    
    fig = px.line(
        df, 
        x="P",
        y="1/time_mean", 
        color="N", 
	    log_y = True,
	    log_x = True,
        title="Strong Scaling", 
        markers=True)
    
    fig.update_layout(
       xaxis = dict(
            tickmode = 'array',
            tickvals = Ps,
        )
    )
    fig.write_image(f"{PLOT_FOLDER}/plot_ss.svg")


if __name__ == "__main__":
    main()


