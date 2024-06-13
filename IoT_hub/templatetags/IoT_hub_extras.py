from django import template
from django.utils import dateformat

register = template.Library()

@register.simple_tag
def get_histogram_offset(max_depth, depth):
    """Calculates the percentage that the histogram bar needs to be offset from the top to correctly show depth data.

    Keyword arguments:
    max_depth -- the maximum depth value in the dataset
    depth -- the depth value for the current data point
    """

    return str(100 - depth/max_depth*100) + '%'

@register.simple_tag
def get_first_date(data):
    """
        Gets first date from a list of Depth objects

        Keyword arguments:
        data -- the list of Depth objects to retrieve the first date from
    """
    return dateformat.DateFormat(data[0].date.astimezone()).format('d/m/y h:i:s A') 

@register.simple_tag
def get_first_depth(data):
    """
        Gets first depth point from a list of Depth objects

        Keyword arguments:
        data -- the list of Depth objects to retrieve the first depth from
    """
    return data[0].depth

@register.simple_tag
def get_axis_height(list):
    """
        Calculates the height of the histogram based on the amount of y-labels and the height of the y-labels

        Keyword arguments:
        list -- the list of y-labels
    """
    return len(list) * 26