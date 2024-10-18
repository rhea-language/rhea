$(document).ready(function() {
    const adjustTabHeight = ()=>
        $('.tab-content').height(
            $('.tab-content .tab-pane.active')
                .outerHeight()
        );

    adjustTabHeight();
    $('a[data-toggle="tab"]').on('shown.bs.tab', ()=>
        adjustTabHeight()
    );
});
