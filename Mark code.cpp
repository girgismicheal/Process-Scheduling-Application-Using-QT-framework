#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <bits/stdc++.h>
using namespace std;

class proces
{
public:
	string name;
	float arriv_time;
	float req_time;
	float real_req_time;
	int priority;
	proces(string id,float arriv_t,float req_t,int pr=1)
	{
		name=id;
		arriv_time = arriv_t;
		req_time = req_t;
		real_req_time = req_t;
		priority = pr;
	}
};
class point{
public:
	float x;
	string y;
	point(float d1,string name)
	{
		x=d1;
		y=name;
	}
};

bool arrive_first (proces p1,proces p2) { return (p1.arriv_time<p2.arriv_time); }
bool shorter (proces p1,proces p2) { return (p1.req_time<p2.req_time); }
bool shorter_original (proces p1,proces p2) { return (p1.real_req_time<p2.real_req_time); }
bool more_important(proces p1,proces p2) { return (p1.priority<p2.priority); }

class my_list
{
public:
	vector<proces> p_list;
	void add_process(string id,float arriv_t,float req_t,int pr=1)
{
	proces nnew = proces(id,arriv_t,req_t,pr);
	p_list.push_back(nnew);
}
	float total_time(vector<proces> pros_list)
{
	float t=0;
	for (int i = 0; i < pros_list.size(); i++)
		t += pros_list[i].req_time;
	return t;
}
	float next_small(float time,vector<proces> pros_list)
{
	for(int i=0;i < pros_list.size();i++)
	{
		if(time>pros_list[i].req_time)
			return pros_list[i].arriv_time;
	}
	return -1;
}
	int next_important(int priority,vector<proces> pros_list)
{
	for(int i=0;i < pros_list.size();i++)
	{
		if(priority>pros_list[i].req_time)
			return pros_list[i].arriv_time;
	}
	return -1;
}
	float next_arrivetime(vector<proces> pros_list)
{
	float min=pros_list[0].arriv_time;
	for(int i=0;i < pros_list.size();i++)
	{
		if(min>pros_list[i].arriv_time)
			min = pros_list[i].arriv_time;
	}
	return min;
}

	void fcfs(vector<point> &points_list)
{
	points_list.clear();
	float t_current=0;
	points_list.push_back(point(t_current,"NULL"));
	sort(p_list.begin(),p_list.end(),arrive_first);
	t_current += p_list[0].arriv_time;
	points_list.push_back(point(t_current,"NULL"));
	for (int i = 0; i < p_list.size(); i++)
	{
		if(p_list[i].arriv_time<=t_current)
		{
			points_list.push_back(point(t_current,p_list[i].name));
			t_current += p_list[i].req_time;
			points_list.push_back(point(t_current,p_list[i].name));
		}
		else
		{
			points_list.push_back(point(t_current,"NULL"));
			t_current = p_list[i].arriv_time;
			points_list.push_back(point(t_current,"NULL"));
			points_list.push_back(point(t_current,p_list[i].name));
			t_current += p_list[i].req_time;
			points_list.push_back(point(t_current,p_list[i].name));

		}
	}
}
	void sjf(vector<point> &points_list)
{
	points_list.clear();
	float t_current=0,t_wait=0,total=total_time(p_list);
	bool flag;
	sort(p_list.begin(),p_list.end(),shorter);
	//cout<<total_time(p_list)<<"\n";
	points_list.push_back(point(t_current,"NULL"));
	t_current=next_arrivetime(p_list);
	points_list.push_back(point(t_current,"NULL"));
	for(;total>0;)
	{
		flag=1;
		for (int j = 0; j < p_list.size(); j++)
		{
			if(p_list[j].arriv_time<=t_current)
			{
				points_list.push_back(point(t_current,p_list[j].name));
				t_current += p_list[j].req_time;
				points_list.push_back(point(t_current,p_list[j].name));
				total -= p_list[j].req_time;
				cout<<p_list[j].name<<" "<<t_current<<"\n";
				p_list.erase(p_list.begin()+j);
				flag=0;
			}
		}
		if(flag==1)
		{
			points_list.push_back(point(t_current,"NULL"));
			t_current=next_arrivetime(p_list);
			points_list.push_back(point(t_current,"NULL"));
		}
	}
}
	void round_robin(float step,vector<point> &points_list)
{
	points_list.clear();
	float t_current=0,t_wait=0,total=total_time(p_list);
	vector<proces> working;
	bool flag;
	sort(p_list.begin(),p_list.end(),arrive_first);
	points_list.push_back(point(t_current,"NULL"));
	t_current=next_arrivetime(p_list);
	points_list.push_back(point(t_current,"NULL"));
	for(;total>0;)
	{
		flag = 1;
		if(p_list.size()>0)
		if(p_list[0].arriv_time<=t_current)
		{
			working.push_back(p_list[0]);
			p_list.erase(p_list.begin());
		}
		for(int i=0;i<working.size();i++)
		{
			flag=0;
			points_list.push_back(point(t_current,working[i].name));
			t_current += step;
			points_list.push_back(point(t_current,working[i].name));
			total -= step;
			//cout<<working[i].name<<" "<<t_current<<"\n";
			working[i].req_time -=step;
			if(working[i].req_time<=0)
				working.erase(working.begin()+i);
		}
		if(flag==1)
		{
			points_list.push_back(point(t_current,"NULL"));
			t_current=next_arrivetime(p_list);
			points_list.push_back(point(t_current,"NULL"));
		}
	}
}
	void priority(vector<point> &points_list)
{
	points_list.clear();
	float t_current=0,t_wait=0,total=total_time(p_list);
	bool flag;
	sort(p_list.begin(),p_list.end(),more_important);
	points_list.push_back(point(t_current,"NULL"));
	t_current=next_arrivetime(p_list);
	points_list.push_back(point(t_current,"NULL"));
	for(;total>0;)
	{
		flag=1;
		for (int j = 0; j < p_list.size(); j++)
		{
			if(p_list[j].arriv_time<=t_current)
			{
				points_list.push_back(point(t_current,p_list[j].name));
				t_current += p_list[j].req_time;
				points_list.push_back(point(t_current,p_list[j].name));
				total -= p_list[j].req_time;
				cout<<p_list[j].name<<" "<<t_current<<"\n";
				p_list.erase(p_list.begin()+j);
				flag=0;
			}
		}
		if(flag==1)
		{
			points_list.push_back(point(t_current,"NULL"));
			t_current=next_arrivetime(p_list);
			points_list.push_back(point(t_current,"NULL"));
		}
	}
}
	void sjf_preemptive(vector<point> &points_list)
{
	points_list.clear();
	float t_current=0,t_wait=0,total=total_time(p_list),n_s;
	bool flag;
	vector<proces> working;
	sort(p_list.begin(),p_list.end(),arrive_first);
	cout<<total_time(p_list)<<"\n";
	for(;total>0;)
	{
		flag = 1;
		for(int i=0;i<p_list.size();i++)
		{
		if(p_list[i].arriv_time<=t_current)
		{
				working.push_back(p_list[i]);
				sort(working.begin(),working.end(),shorter);
				p_list.erase(p_list.begin()+i);
				i--;
		}
		}
		if(working.size()!=0)
		{
			n_s = next_small(working[0].real_req_time,p_list);
			flag=0;
			cout<<t_current<<" ";
			if(n_s>0)
			{
				if(n_s<(working[0].req_time+t_current))
				{
					total -= (n_s-t_current);
					working[0].req_time -=(n_s-t_current);
					points_list.push_back(point(t_current,working[0].name));
					t_current = n_s;
					points_list.push_back(point(t_current,working[0].name));
				}
				else
				{
					total -=working[0].req_time;
					points_list.push_back(point(t_current,working[0].name));
					t_current += working[0].req_time;
					points_list.push_back(point(t_current,working[0].name));
					working[0].req_time=0;

				}
			}
			if(n_s==-1)
			{
				total -= working[0].req_time;
				points_list.push_back(point(t_current,working[0].name));
				t_current += working[0].req_time;
				points_list.push_back(point(t_current,working[0].name));
				working[0].req_time = 0;
			}
			cout<<working[0].name<<" "<<t_current<<"\n";
			if(working[0].req_time<=0)
				working.erase(working.begin());
		}
		if(flag==1)
		{
			points_list.push_back(point(t_current,"NULL"));
			t_current=next_arrivetime(p_list);
			points_list.push_back(point(t_current,"NULL"));
		}
	}
}
	void priority_preemptive(vector<point> &points_list)
{
	{
	points_list.clear();
	float t_current=0,t_wait=0,total=total_time(p_list),n_s;
	bool flag;
	vector<proces> working;
	sort(p_list.begin(),p_list.end(),arrive_first);
	cout<<total_time(p_list)<<"\n";
	for(;total>0;)
	{
		flag = 1;
		for(int i=0;i<p_list.size();i++)
		{
		if(p_list[i].arriv_time<=t_current)
		{
				working.push_back(p_list[i]);
				sort(working.begin(),working.end(),more_important);
				p_list.erase(p_list.begin()+i);
				i--;
		}
		}
		if(working.size()!=0)
		{
			n_s = next_important(working[0].priority,p_list);
			flag=0;
			cout<<t_current<<" ";
			if(n_s>0)
			{
				if(n_s<(working[0].req_time+t_current))
				{
					total -= (n_s-t_current);
					working[0].req_time -=(n_s-t_current);
					points_list.push_back(point(t_current,working[0].name));
					t_current = n_s;
					points_list.push_back(point(t_current,working[0].name));
				}
				else
				{
					total -=working[0].req_time;
					points_list.push_back(point(t_current,working[0].name));
					t_current += working[0].req_time;
					points_list.push_back(point(t_current,working[0].name));
					working[0].req_time=0;

				}
			}
			if(n_s==-1)
			{
				total -= working[0].req_time;
				points_list.push_back(point(t_current,working[0].name));
				t_current += working[0].req_time;
				points_list.push_back(point(t_current,working[0].name));
				working[0].req_time = 0;
			}
			cout<<working[0].name<<" "<<t_current<<"\n";
			if(working[0].req_time<=0)
				working.erase(working.begin());
		}
		if(flag==1)
		{
			points_list.push_back(point(t_current,"NULL"));
			t_current=next_arrivetime(p_list);
			points_list.push_back(point(t_current,"NULL"));
		}
	}
}
}
};



int main()
{
	vector<point> points;
	float step=0.5; //for round robin
	my_list p_list;
	p_list.add_process("p1",0.9,5,3);p_list.add_process("p2",0.5,4,2);p_list.add_process("p3",1,1,1);
	p_list.round_robin(step,points);
	cout<<"**************\n";
	for(int i=0;i<points.size();i++)
	{
		cout<<points[i].x<<" "<<points[i].y<<"\n";
	}
	system ("pause");
	return 0;
}