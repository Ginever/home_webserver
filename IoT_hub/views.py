from django.shortcuts import render
from .models import WaterTankDepth
from django.utils import dateformat
from math import ceil



def currentDepth(request):
    """
    A view is to display the current depth of the water tank
    """
    latest_data = WaterTankDepth.objects.latest('date')
    latest_data_list = WaterTankDepth.objects.order_by('-date')[:100]
    return render(request, "IoT_hub/currentDepth.html", {
        'depth': round(latest_data.depth, 2),
        #amount of water in the water tank in liters calculated using tank dimensions duracreate 24,000L concrete
        'water_liters': round((1000 * 3.14 * (2.635 - latest_data.depth/100 - 0.01) * (3.73**2 - 0.15**2))/4),
        'date': dateformat.DateFormat(latest_data.date.astimezone()).format('d/m/y h:i:s A'),
        'depth_list': latest_data_list,
        'max_depth': ceil(max(data.depth for data in latest_data_list)/10) * 10,
        'date_label': len(latest_data_list)/20,
        #A list of labels to put on the y-axis of the histogram
        'histogram_y_labels': [i*10 for i in range(1,ceil(max(data.depth for data in latest_data_list)/10 + 1),2)]
        })

