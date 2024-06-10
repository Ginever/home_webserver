from django import template
from django.utils import dateformat

register = template.Library()

@register.simple_tag
def get_histogram_offset(max_depth, depth):
    return str(100 - depth/max_depth*100) + '%'

@register.simple_tag
def get_first_date(data):
    return dateformat.DateFormat(data[0].date.astimezone()).format('d/m/y h:i:s A') 

@register.simple_tag
def get_first_depth(data):
    return data[0].depth

@register.simple_tag
def check_if_mod(context, a):
    return a % context['date_label'] == 0

@register.simple_tag
def get_axis_height(list):
    return len(list) * 26