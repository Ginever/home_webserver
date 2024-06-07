from django.shortcuts import render
from .models import WaterTankDepth
from django.utils import dateformat

def currentDepth(request):
    latest_data = WaterTankDepth.objects.latest('date')
    return render(request, "IoT_hub/currentDepth.html", {
        'depth': round(latest_data.depth, 2), 
        'date': dateformat.DateFormat(latest_data.date.astimezone()).format('d/m/y h:i:s A')
        })

