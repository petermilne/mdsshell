cat - <<EOF 
Scope.plot_${ROW}_${COL}.height: 65
Scope.plot_${ROW}_${COL}.grid_mode: 0
Scope.plot_${ROW}_${COL}.x_log: false
Scope.plot_${ROW}_${COL}.y_log: false
Scope.plot_${ROW}_${COL}.update_limits: true
Scope.plot_${ROW}_${COL}.experiment: ${TREE}
Scope.plot_${ROW}_${COL}.num_expr: ${NUMEXPR}
Scope.plot_${ROW}_${COL}.num_shot: ${SHOT}
Scope.plot_${ROW}_${COL}.shot: 0
Scope.plot_${ROW}_${COL}.global_defaults: 261633
Scope.plot_${ROW}_${COL}.color_${ROW}_${COL}: 0
Scope.plot_${ROW}_${COL}.marker_${ROW}_${COL}: 0
Scope.plot_${ROW}_${COL}.step_marker_${ROW}_${COL}: 1
EOF

if [ "$EVENT" != "" ]
then
echo Scope.plot_${ROW}_${COL}.event: $EVENT
fi

expression() {
    echo Scope.plot_${ROW}_${COL}.y_expr_$1: $2
}
